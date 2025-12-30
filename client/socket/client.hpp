#pragma once
#include <iostream>


class Client
{
private:
    std::string host;
    int sockfd;
    int port;
    void createSocket();
public:
    Client(std::string host, int port);
    void connectServer();
    void commWithServer();
};