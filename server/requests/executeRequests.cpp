#include "executeRequests.hpp"
#include "../functions/database-handling.hpp"
#include "../classes/UserAccount_server.hpp"
#include <iostream>
#include <string>
using namespace std;

static int sockfd = -1;

void readRequests_init(int passed_sockfd)
{
    sockfd = passed_sockfd;
}

void requests::user::login(Packet packet)
{
    string username = packet.getUsername();
    string password = packet.getPassword();

    Packet response;

    if (checkUniqueUsername(username))
    {
        // username not found in DB (unique = doesn't exist)
        response = Packet("USER-NOT-FOUND");
    }
    else if (checkUserPassword(username, password))
    {
        // username exists AND password matches
        UserAccount_server user = getUserByUsername(username);
        Packet successPacket("LOGIN-SUCCESS", user);
        successPacket.write(sockfd);
        return;
    }
    else
    {
        // username exists but wrong password
        response = Packet("LOGIN-FAIL");
    }

    response.write(sockfd);
}

void requests::user::signup(Packet p)
{

    UserAccount_server acc;
    acc = p;        // overloaded the UserAccount class to support ' = ' operations with Packet class
    acc.printAccountInfo(0);
    acc.setAccountNo(generateAccountNo());
    cout << "generated acc no \n";
    acc.setClientID(acc.generateClientID());
    acc.printAccountInfo(0);

    // enter dbms signup code here
}

void requests::user::forgotPassword(Packet packet)
{
    string username = packet.getUsername();

    // Check user exists
    if(checkUniqueUsername(username))  // true = doesn't exist
    {
        Packet response("USER-NOT-FOUND");
        response.write(sockfd);
        return;
    }

    // User exists — tell client to proceed
    Packet response("FOUND");
    response.write(sockfd);

    // Now wait for favAni answer
    Packet answerPacket;
    answerPacket.read(sockfd);

    // TODO: when DB is ready, fetch real favAni and compare
    // For now, stub: always wrong (teammates fill this in)
    string storedFavAni = "";  // TODO: getUserByUsername(username).getFavAni()
    string givenFavAni  = answerPacket.getFavAni();

    if(givenFavAni != storedFavAni)
    {
        Packet wrong("WRONG-ANSWER");
        wrong.write(sockfd);
        return;
    }

    Packet verified("VERIFIED");
    verified.write(sockfd);

    // Wait for new password
    Packet changePacket;
    changePacket.read(sockfd);

    string newPassword = changePacket.getPassword();

    // TODO: updatePassword(username, newPassword) — teammates implement
    
    Packet done("PASS-CHANGED");
    done.write(sockfd);
}