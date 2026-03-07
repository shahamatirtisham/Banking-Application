#include "executeRequests.hpp"
#include "../functions/database-handling.hpp"
#include "../classes/UserAccount_server.hpp"
#include <iostream>
#include <string>
using namespace std;

static int sockfd = -1;
static PGconn* connection = NULL;

void exeRequests_init(int passed_sockfd, PGconn* passed_connection)
{
    sockfd = passed_sockfd;
    connection = passed_connection;
}

void requests::user::login(Packet packet)
{
//     string username = packet.getUsername();
//     string password = packet.getPassword();

//     Packet response;

//     if (checkUniqueUsername(username))
//     {
//         // username not found in DB (unique = doesn't exist)
//         response = Packet("USER-NOT-FOUND");
//     }
//     else if (checkUserPassword(username, password))
//     {
//         // username exists AND password matches
//         UserAccount_server user = getUserByUsername(username);
//         Packet successPacket("LOGIN-SUCCESS", user);
//         successPacket.write(sockfd);
//         return;
//     }
//     else
//     {
//         // username exists but wrong password
//         response = Packet("LOGIN-FAIL");
//     }

//     response.write(sockfd);
}

bool requests::user::checkUniqueUsername(string username)
{
    Packet response;
    User_Queries query(connection);
    return query.checkUniqueUsername(username);    
}

void requests::user::signup(Packet p)
{

    // overloaded the UserAccount class to support assignment operations with Packet class
    UserAccount_server acc;
    acc = p;
    acc.printAccountInfo(0);
    acc.setAccountNo(generateAccountNo());
    cout << "generated acc no \n";
    acc.setClientID(acc.generateClientID());
    acc.printAccountInfo(0);

    
    User_Queries db(connection);
    db.addUser(acc);
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



void requests::user::check_balance(Packet packet)
{

}

void requests::user::deposit(Packet packet)
{

}

void requests::user::withdraw(Packet packet)
{

}

void requests::user::transfer_money(Packet packet)
{

}

void requests::user::transaction_history(Packet packet)
{

}

void requests::user::logout(Packet packet)
{

}
