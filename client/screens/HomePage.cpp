#include <iostream>
#include "../../shared/classes/UserAccount.hpp"
#include "../../shared/classes/Packet.hpp"
#include "HomePage.hpp"
#include "../shared/input.hpp"
#include "../classes/TextBox.hpp"
#include <stdexcept>
#include <iomanip>
#include <vector>
using namespace std;


// > ▶
// ✔ ✖
// ● ○
// ■ □
// ▲ ▼

HomePage::HomePage(int sockfd, UserAccount user) 
    : sockfd(sockfd), operation(sockfd), printBal(false), notice("")
{
    this->user = user;
    
    Packet p("GET-ACC&BAL", user);
    // cout << "writing to get bal\n";
    p.write(this->sockfd);
    // cout << "reading to get bal\n";
    p.read(this->sockfd);
    // cout << "done reading to get bal\n";
    string accNo = p.getAccountNo();
    this->user.setAccountNo(accNo);
    double balance = p.getBalance();
    this->user.setBalance(balance);
    
}

void HomePage::printHeader()
{

    
    if(printBal)
    {
        stringstream _balance;
        _balance << fixed << setprecision(2) << user.getBalance();
        vector<string> strs = 
        {
            "USER DASHBOARD",
            "Welcome " + user.getUsername() + "!",
            "Account number: " + user.getAccountNo(),
            "Balance: Tk" + _balance.str()
        };
        TextBox tb_header(strs, 40);
    }
    else
    {
        vector<string> strs = 
        {
            "USER DASHBOARD",
            "Welcome " + user.getUsername() + "!",
            "Account number: " + user.getAccountNo()
        };
        TextBox tb_header(strs, 40);
    }

    if(printBal == true)
        printBal = false;



    // cout <<"Welcome, " <<user.getUsername() <<"!" <<endl;
    // cout <<"Account Number: " <<user.getAccountNo() <<endl <<endl;
}

void HomePage::printMenu()
{
    vector<string> strs = 
    {
        "1. Check Balance",
        "2. Deposit",
        "3. Withdraw",
        "4. Transfer Money",
        "5. Transaction History",
        "6. Logout"       
    };

    TextBox tb_options(strs, 40, false, 3);
}

void HomePage::display()
{
    // system("clear");
    printHeader();
    printMenu();
    if(notice != "")
    {
        cout << notice << endl;
        notice = "";
    }
}

int HomePage::getChoice()
{
    int choice; 
    while(true)
    {
        cout <<"▶ Enter Choice: "; choice = readInt();
        if(choice <= 6 && choice >= 1) break;
        else cout <<"Invalid Choice. Try Again." <<endl;
    }
    return choice;
}

void HomePage::userOperation(int choice)
{
    if(choice == 1) 
    {
        try
        {
            clearScreen();
            operation.check_balance(user);
            printBal = true;
        }
        catch(const exception &e)
        {
            cout <<"Error Occured : " <<e.what() <<endl;
        }
    }
    else if(choice == 2) 
    {
        try
        {
            int res = operation.deposit(user);
            switch (res)
            {
            case 0:    notice = "";

            case -1:     notice = "✖ Insufficient balance, transfer failed";
                        break;
            case 1:     notice = "✔ Deposition success!";
                        break;
            default:    notice = "✖ Something went wrong, try again later\n";
            }
        }
        catch(const exception &e)
        {
            cout <<"Error Occured : " <<e.what() <<endl;
        }
    }
    else if(choice == 3) 
    {
        try
        {
            int res = operation.withdraw(user);

            switch (res)
            {
            case 0:    notice = "";

            case -1:     notice = "✖ Insufficient balance, transfer failed";
                        break;
            case 1:     notice = "✔ Withdrawal success!";
                        break;
            default:    notice = "✖ Something went wrong, try again later\n";
            }
            
            
        }
        catch(const exception &e)
        {
            cout <<"Error Occured : " <<e.what() <<endl;
        }
    }
    else if(choice == 4) 
    {
        try
        {
            int res = operation.transfer_money(user);

            switch (res)
            {
            case 0:    notice = "";

            case -2:    notice = "✖ Account number not found";
                        break;
            case -1:     notice = "✖ Insufficient balance, transfer failed";
                        break;
            case 1:     notice = "✔ Transfer success!";
                        break;
            default:    notice = "✖ Something went wrong, try again later\n";
            }
        }
        catch(const exception &e)
        {
            cout <<"Error Occured : " <<e.what() <<endl;
        }
    }
    else if(choice == 5) 
    {
        try
        {
            operation.transaction_history(user);
        }
        catch(const exception &e)
        {
            cout <<"Error Occured : " <<e.what() <<endl;
        }
    }
    else if(choice == 6) 
    {
        try
        {
            operation.logout(user);
        }
        catch(const exception &e)
        {
            cout <<"Error Occured : " <<e.what() <<endl;
        }
    }
}