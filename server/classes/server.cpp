#include "../../shared/classes/Packet.hpp"
#include "../requests/readRequests.hpp"
#include <iostream>
#include <strings.h>
#include <unistd.h>
#include <netinet/in.h>
using namespace std;

class Server
{
private:    
    int port;
    int listenSock;
    void setupSocket();
    void acceptLoop();
    void handleClient(int clientSock);

public:
    Server(int port);
    void run();
};

Server::Server(int port) : port(port), listenSock(-1) {};

void Server::setupSocket()
{
    cout << "Server running.\n";

    struct sockaddr_in server_address, client_address;
    socklen_t client_len;

    listenSock = socket(AF_INET, SOCK_STREAM, 0);
    if(listenSock < 0)
    {
        perror("Error opening socket.");
    }

    bzero((char *) &server_address, sizeof(server_address));      //sets all bytes in the server_address block to be zero

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(port);

    if(bind(listenSock, (struct sockaddr *) &server_address, sizeof(server_address)) < 0)
    {
        perror("Binding failed");
    }

    listen(listenSock, 5);
}

void Server::acceptLoop()
{
    while(true)
    {
        struct sockaddr_in clientAddr;
        socklen_t client_len = sizeof(clientAddr);
        int clientSock = accept(listenSock, (struct sockaddr *) &clientAddr, &client_len);

        if(client_len < 0)
            continue;

        pid_t pid = fork();

        if(pid == 0)    // child process has pid 0, it mustnt listen for new clients, therefore closes the listening socket 
        {
            close(listenSock);   
            handleClient(clientSock);
            close(clientSock);
            exit(0);
        }
        else            // parent process mustnt handle client, therefore closes the client socket
        {
            close(clientSock);
        }

    }
}

void Server::run()
{
    setupSocket();
    acceptLoop();
}

void Server::handleClient(int clientSocket)
{
    std::cout << "Handling client\n";
    readRequests_init(clientSocket);
    cout << "check1\n";
    while(1)
    {
        cout << "check2\n";
        Packet p;
        cout << "check3\n";
        p.read(clientSocket);

        // packet.display();
        string command = p.getCommand();
        p.display();
        
    
        if(command == "SIGNUP")
        {
            requests::user::signup(p);
        }
        else if(command == "UNIQUE-USERNAME-CHECK")
        {
            if(checkUniqueUsername(p.getUsername()) == true)
            {
                p = Packet("POSITIVE");
                cout << "username is unique\n";
            }
            else
            {
                p = Packet("NEGATIVE");
                cout << "username is taken\n";
            }
            p.display();
            p.write(clientSocket);
        }
    }    
}