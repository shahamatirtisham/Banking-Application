#pragma once
#include <string>
using namespace std;

class Client
{
private:
    std::string host;
    int sockfd;
    int port;
    void createSocket();
public:
    Client(std::string host, int port);
    int getSockfd() const;
    bool connectServer();
};




