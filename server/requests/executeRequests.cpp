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

    DatabaseUpdates db(connection);
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
    string username = packet.getUsername();
    
    User_Queries db(connection);
    double balance = db.getBalance(username); // database theke current balance pailam

    UserAccount user;
    user.setBalance(balance); // packet er moddhe  balance set korlam
    Packet p("CURRENT-BALANCE", user);
    p.write(sockfd);
}

void requests::user::deposit(Packet packet)
{
    string username = packet.getUsername();
    double amount = packet.getBalance(); // eita balance na, eita deposit amount

    User_Queries db(connection);
    DatabaseUpdates dbu(connection);
    
    double balance = db.getBalance(username); // database theke current balance pailam
    double newBalance = balance + amount; // amount deposit korlam
    
    bool success = dbu.updateBalance(username, newBalance); // database e balance update korlam
    while(!success) 
    {
        success = dbu.updateBalance(username, newBalance); 
    }

    UserAccount user;
    user.setBalance(newBalance); // packet er moddhe  newBalance set korlam
    Packet p("DEPOSIT-SUCCESS", user);
    p.write(sockfd);
}

void requests::user::withdraw(Packet packet)
{
    string username = packet.getUsername();
    double amount = packet.getBalance(); // eita balance na, eita withdraw amount

    User_Queries db(connection);
    bool enoughBalance = db.hasEnoughBalance(username, amount);

    if(enoughBalance)
    {
        DatabaseUpdates dbu(connection);

        double balance = db.getBalance(username); // database theke current balance pailam
        double newBalance = balance - amount; // amount withdraw korlam

        bool success = dbu.updateBalance(username, newBalance); // database e balance update korlam
        while(!success) 
        {
            success = dbu.updateBalance(username, newBalance); 
        }

        UserAccount user;
        user.setBalance(newBalance); // packet er moddhe  newBalance set korlam
        Packet p("WITHDRAW-SUCCESS", user); 
        p.write(sockfd);
    }
    else // jodi enough balance na thake taile...
    {
        UserAccount user;
        Packet p("INSUFFICIENT-BALANCE", user); 
        p.write(sockfd);
    }
}

void requests::user::transfer_money(Packet packet)
{
    string senderUserName = packet.getUsername(); // sender er username
    string receiverAccNo = packet.getAccountNo(); // receiver er account no
    double amount = packet.getBalance(); // eita balance na, eita transfer amount

    User_Queries db(connection);
    DatabaseUpdates dbu(connection);

    // prothome check korbo receiver acc no valid kina
    bool receiverDoesNotExist = db.checkUniqueAccountNo(receiverAccNo);
    if(receiverDoesNotExist)
    {
        UserAccount user;
        Packet p("RECEIVER-NOT-FOUND", user);
        p.write(sockfd);
        return;
    }

    // erpor check korbo sender er enough balance ase kina
    bool senderHasEnoughBalance = db.hasEnoughBalance(senderUserName, amount);
    if(!senderHasEnoughBalance)
    {
        UserAccount user;
        Packet p("INSUFFICIENT-BALANCE", user); 
        p.write(sockfd);
        return;
    }

    // sender theke deduct kore, receiver e add korbo, database e update korbo
    string receiverUserName = db.getUsernameByAccountNo(receiverAccNo);

    double senderBalance = db.getBalance(senderUserName);
    double receiverBalance = db.getBalance(receiverUserName);

    double senderNewBalance = senderBalance - amount;
    double receiverNewBalance = receiverBalance + amount;

    bool senderUpdated = dbu.updateBalance(senderUserName, senderNewBalance);
    while(!senderUpdated)
    {
        senderUpdated = dbu.updateBalance(senderUserName, senderNewBalance);
    }
    
    bool receiverUpdated = dbu.updateBalance(receiverUserName, receiverNewBalance);
    while(!receiverUpdated)
    {
        receiverUpdated = dbu.updateBalance(receiverUserName, receiverNewBalance);
    }
    
    UserAccount user;
    user.setBalance(senderNewBalance);
    Packet p("TRANSFER-SUCCESS", user);
    p.write(sockfd);
}

void requests::user::transaction_history(Packet packet)
{
    string username = packet.getUsername();
    // database theke vector return korbo
}

void requests::user::logout(Packet packet)
{
    // eita baki roilo
}