#include "client.hpp"
#include <iostream>
#include <strings.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

using namespace std;

Client::Client(string host, int port)
{
    this->host = host;
    this->port = port;
}

void Client::createSocket()
{
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0)
    {
        perror("Error opening socket.");
    }
    else
    {
        cout << "Socket creation success\n";
    }
}

bool Client::connectServer()
{
    createSocket();
    
    struct hostent* server = gethostbyname(host.c_str());
    if(server == nullptr)
    {
        fprintf(stderr, "Error: No such host\n");
        return false;
    }
    
    struct sockaddr_in server_address;
    bzero((char *) &server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    bcopy((char *) server->h_addr_list[0], (char *)&server_address.sin_addr.s_addr, server->h_length);
    server_address.sin_port = htons(port);
    
    if(connect(sockfd, (struct sockaddr *) &server_address, sizeof(server_address)) < 0)
    {
        perror("Connection failed.");
        return false;
    }
    else
    {
        cout << "Connect success\n";
        return true;
    }
}

int Client::getSockfd() const
{
    return sockfd;
}


