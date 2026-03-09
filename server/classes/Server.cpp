#include "Server.hpp"
#include "../../shared/classes/Packet.hpp"
#include "../requests/executeRequests.hpp"
#include "../functions/database-handling.hpp"
#include "../classes/BankData.hpp"
#include <iostream>
#include <strings.h>
#include <unistd.h>
#include <netinet/in.h>
using namespace std;


Server::Server(int port) : port(port), listenSock(-1) {}

void Server::setupSocket()
{
    struct sockaddr_in server_address, client_address;
    socklen_t client_len;

    

    listenSock = socket(AF_INET, SOCK_STREAM, 0);
    if (listenSock < 0)
    {
        perror("Error opening socket.");
        exit(1);
    }

    bzero((char *)&server_address, sizeof(server_address)); // sets all bytes in the server_address block to be zero

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(port);


    if (bind(listenSock, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        perror("Binding failed");
        exit(1);
    }


    listen(listenSock, 5);
}

void Server::acceptLoop()
{
    while (true)
    {
        struct sockaddr_in clientAddr;
        socklen_t client_len = sizeof(clientAddr);
        int clientSock = accept(listenSock, (struct sockaddr *)&clientAddr, &client_len);

        if (client_len < 0)
            continue;

        pid_t pid = fork();

        if (pid == 0) // child process has pid 0, it mustnt listen for new clients, therefore closes the listening socket
        {
            close(listenSock);
            handleClient(clientSock);
            close(clientSock);
            exit(0);
        }
        else // parent process mustnt handle client, therefore closes the client socket
        {
            close(clientSock);
        }
    }
}

void Server::setupDatabase()
{
    connection = config_info.connect("bankdb");
    if(!connection)
    {
        cout << "Connection to the database failed, exiting program\n";
        exit(1);
    }
    Database db(connection, 1, "");
}

void Server::run()
{
    setupSocket();    
    BankData bankData;
    setupDatabase();
    bankData.display();
    acceptLoop();
}

void Server::handleClient(int clientSocket)
{
    std::cout << "Handling client\n";
    exeRequests_init(clientSocket, connection);
    while (1)
    {
        Packet p;
        p.read(clientSocket);

        string command = p.getCommand();
        p.display();

        if (command == "LOGIN")
        {
            requests::user::login(p);
        }

        else if (command == "SIGNUP")
        {
            requests::user::signup(p);
        }
        else if (command == "FORGOT-PASS")
        {
            requests::user::forgotPassword(p);
        }
        else if (command == "CHANGE-PASS")
        {
            requests::user::changePassword(p);
        }
        else if (command == "UNIQUE-USERNAME-CHECK")
        {
            bool response = requests::user::checkUniqueUsername(p.getUsername());
            response ? p = Packet("POSITIVE") : p = Packet("NEGATIVE");
            p.write(clientSocket);
        }
        else if(command == "CHECK-BALANCE")
        {
            requests::user::check_balance(p);
        }
        else if(command == "DEPOSIT")
        {
            requests::user::deposit(p);
            // ekhane amount er kaj baki ase
        }
        else if(command == "WITHDRAW")
        {
            requests::user::withdraw(p);
            // ekhane amount er kaj baki ase
        }
        else if(command == "TRANSFER-MONEY")
        {
            requests::user::transfer_money(p);
            // ekhane amount er kaj baki ase
            // ekhane receiver account er kaj baki ase

        }
        else if(command == "TRANSACTION-HISTORY")
        {
            requests::user::transaction_history(p);
        }
        else if(command == "LOGOUT")
        {
            requests::user::logout(p);
        }
    }    
}