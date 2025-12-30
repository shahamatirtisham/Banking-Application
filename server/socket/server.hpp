#pragma once



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