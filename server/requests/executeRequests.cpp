#include "executeRequests.hpp"
#include "../functions/database-handling.hpp"
#include "../classes/UserAccount_server.hpp"
#include <iostream>
#include <string>
#include "../functions/logger.hpp"

using namespace std;

static int sockfd = -1;
static PGconn *connection = NULL;

void exeRequests_init(int passed_sockfd, PGconn *passed_connection)
{
    sockfd = passed_sockfd;
    connection = passed_connection;
}

void requests::user::login(const Packet &packet)
{
    string username = packet.getUsername();
    string password = packet.getPassword();

    User_Queries q(connection);

    Packet response;

    bool usernameExists = !q.checkUniqueUsername(username);
    string db_password = q.getPasswordByUsername(username);

    usernameExists ? cout << "Username exists\n" : cout << "Username does not exist\n";
    cout << "Password in DB of the relevant username: " << db_password << endl;

    if (usernameExists && db_password == password)
    {
        response = Packet("POSITIVE");
        ActivityLogger aLogger;
        aLogger.logLogin(username);
    }
    else
    {
        response = Packet("NEGATIVE");
    }

    response.write(sockfd);
}

bool requests::user::checkUniqueUsername(const string &username)
{
    Packet response;
    User_Queries query(connection);
    return query.checkUniqueUsername(username);
}

void requests::user::signup(const Packet &p)
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
    ActivityLogger aLogger;
    aLogger.logSignup(acc.getUsername());
}

void requests::user::forgotPassword(const Packet &packet)
{
    string username = packet.getUsername();
    Date DOB = packet.getDOB();
    string favAni = packet.getFavAni();

    // Check user exists
    User_Queries q(connection);
    bool userExists = !q.checkUniqueUsername(username);

    if (!userExists)
    {
        Packet response("NEGATIVE");
        response.write(sockfd);
        return;
    }
    cout << "gettin server account\n";
    UserAccount_server db_user = q.getUserAccount_server(username);
    cout << "gotten server account\n";
    Date db_DOB = db_user.getDOB();
    string db_favAni = db_user.getFavAni();

    if (DOB == db_DOB && favAni == db_favAni)
    {
        ActivityLogger aLogger;
        aLogger.logForgotPassword(username);
        Packet verification("POSITIVE");
        verification.write(sockfd);
        return;
    }
    else
    {
        Packet verification("NEGATIVE");
        verification.write(sockfd);
        return;
    }
}
void requests::user::changePassword(const Packet &packet)
{
    DatabaseUpdates dbu(connection);
    string username = packet.getUsername();
    string password = packet.getPassword();
    bool success = dbu.updatePasswordByUsername(username, password, "");
    success ? cout << "Password changed successfully\n" : cout << "Password change failed\n";
    if (success)
    {
        ActivityLogger aLogger;
        aLogger.logPasswordChange(username);
    }
    Packet response;
    success ? response = Packet("POSITIVE") : response = Packet("NEGATIVE");
    response.write(sockfd);
}

void requests::user::check_balance(const Packet &packet)
{
    string username = packet.getUsername();

    User_Queries db(connection);
    double balance = db.getBalance(username); // database theke current balance pailam

    UserAccount user;
    user.setBalance(balance); // packet er moddhe  balance set korlam
    Packet p("CURRENT-BALANCE", user);
    p.write(sockfd);
}

void requests::user::deposit(const Packet &packet)
{
    string username = packet.getUsername();
    double amount = packet.getBalance(); // eita balance na, eita deposit amount

    User_Queries db(connection);
    DatabaseUpdates dbu(connection);

    double balance = db.getBalance(username); // database theke current balance pailam
    double newBalance = balance + amount;     // amount deposit korlam

    bool success = dbu.updateBalance(username, newBalance); // database e balance update korlam
    while (!success)
    {
        success = dbu.updateBalance(username, newBalance);
    }

    UserAccount user;
    user.setBalance(newBalance); // packet er moddhe  newBalance set korlam
    Packet p("DEPOSIT-SUCCESS", user);
    p.write(sockfd);
    TransactionLogger tLogger;            
    tLogger.logDeposit(username, amount); 
}

void requests::user::withdraw(const Packet &packet)
{
    string username = packet.getUsername();
    double amount = packet.getBalance(); // eita balance na, eita withdraw amount

    User_Queries db(connection);
    bool enoughBalance = db.hasEnoughBalance(username, amount);

    if (enoughBalance)
    {
        DatabaseUpdates dbu(connection);

        double balance = db.getBalance(username); // database theke current balance pailam
        double newBalance = balance - amount;     // amount withdraw korlam

        bool success = dbu.updateBalance(username, newBalance); // database e balance update korlam
        while (!success)
        {
            success = dbu.updateBalance(username, newBalance);
        }

        UserAccount user;
        user.setBalance(newBalance); // packet er moddhe  newBalance set korlam
        Packet p("WITHDRAW-SUCCESS", user);
        p.write(sockfd);
        TransactionLogger tLogger;             
        tLogger.logWithdraw(username, amount); 
    }
    else // jodi enough balance na thake taile...
    {
        UserAccount user;
        Packet p("INSUFFICIENT-BALANCE", user);
        p.write(sockfd);
    }
}

void requests::user::transfer_money(const Packet &packet)
{
    string senderUserName = packet.getUsername(); // sender er username
    string receiverAccNo = packet.getAccountNo(); // receiver er account no
    double amount = packet.getBalance();          // eita balance na, eita transfer amount

    User_Queries db(connection);
    DatabaseUpdates dbu(connection);

    // prothome check korbo receiver acc no valid kina
    bool receiverDoesNotExist = db.checkUniqueAccountNo(receiverAccNo);
    if (receiverDoesNotExist)
    {
        UserAccount user;
        Packet p("RECEIVER-NOT-FOUND", user);
        p.write(sockfd);
        return;
    }

    // erpor check korbo sender er enough balance ase kina
    bool senderHasEnoughBalance = db.hasEnoughBalance(senderUserName, amount);
    if (!senderHasEnoughBalance)
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
    while (!senderUpdated)
    {
        senderUpdated = dbu.updateBalance(senderUserName, senderNewBalance);
    }

    bool receiverUpdated = dbu.updateBalance(receiverUserName, receiverNewBalance);
    while (!receiverUpdated)
    {
        receiverUpdated = dbu.updateBalance(receiverUserName, receiverNewBalance);
    }

    UserAccount user;
    user.setBalance(senderNewBalance);
    Packet p("TRANSFER-SUCCESS", user);
    p.write(sockfd);
    TransactionLogger tLogger;                                  
    tLogger.logTransfer(senderUserName, receiverAccNo, amount); 
}

void requests::user::transaction_history(const Packet &packet)
{
    string username = packet.getUsername();
    // database theke vector return korbo
}

void requests::user::logout(const Packet &packet)
{
    // eita baki roilo
    ActivityLogger aLogger;                  
    aLogger.logLogout(packet.getUsername()); 
}