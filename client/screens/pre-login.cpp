#include <iostream>
#include "pre-login.hpp"
#include "../../shared/input.hpp"
#include "../../shared/classes/Packet.hpp"
#include "../../shared/classes/UserAccount.hpp"
#include "../../shared/hash.hpp"
#include "../../server/functions/database-handling.hpp"
#include "homepage-screen.hpp"
#include "../classes/TextBox.hpp"

using namespace std;

static int sockfd = -1;

namespace preloginScreens::user
{
    void signup();
    void login();
}

namespace preloginScreens::admin
{
    void signup();
    void login();
}

void screens_init(int passed_sockfd)
{
    if (passed_sockfd >= 0)
    {
        sockfd = passed_sockfd;
    }
    else
    {
        perror("sockfd not intialized\n");
    }
}

void mainMenu()
{
    while(1)
    {
        clearScreen();
        TextBox welcomeBox("WELCOME TO LONDU BANK", 38);
        // cout << "constructor\n";
        cout << welcomeBox.generateBox();
        // cout << "genned one\n";
        vector<string> lines(2);
        lines[0] = "1. Login as an existing user";
        lines[1] = "2. Signup as a new user";

        TextBox options(lines, 0, false, 3);
        cout << options.generateBox();
        int choice;
        cout << "▶ choice: ";
        cin >> choice;

        switch(choice)
        {
            case  1:    clearScreen(); 
                        preloginScreens::user::login();
                        break;

            case  2:    clearScreen(); 
                        preloginScreens::user::signup();
                        break;

            case  3:    clearScreen(); 
                        preloginScreens::admin::signup();
                        break;

            case  4:    clearScreen(); 
                        preloginScreens::admin::login();
                        break;

            default:    clearScreen(); 
                        cout << "Invalid choice. Try again.\n";
        }
    }
}

void preloginScreens::user::login()
{
    string username, password;

    while (true)
    {
        cout << " -------- LOGIN -------- \n";

        // cout << "Enter your username: ";
        username = acceptUsername();

        // cout << "Enter your password: ";
        password = acceptPassword();

        // Build and send login packet
        UserAccount tempAccount;
        tempAccount.setUsername(username);
        tempAccount.setPassword(password);

        Packet loginPacket("LOGIN", tempAccount);
        loginPacket.write(sockfd);

        // Read server response
        Packet response;
        response.read(sockfd);

        string cmd = response.getCommand();

        if (cmd == "POSITIVE")
        {
            clearScreen();
            cout << "Login Successful! Welcome, " << username << "!\n";
            homepage_init(sockfd);
            homepage_Menu(tempAccount);
            return;
        }
        else if (cmd == "NEGATIVE")
        {
            while (true)
            {
                clearScreen();
                cout << "Incorrect password for: " << username << "\n\n";
                cout << "1. Retry password\n";
                cout << "2. Forgot password\n";
                cout << "0. Back to main menu\n";
                cout << "Select: ";
                int choice = readInt();

                if (choice == 0)
                    return;

                else if (choice == 1)
                {
                    // cout << "Enter password: ";
                    password = acceptPassword();
                    tempAccount.setPassword(password);
                    Packet retryPacket("LOGIN", tempAccount);
                    retryPacket.write(sockfd);

                    response.read(sockfd);
                    cmd = response.getCommand();
                    

                    if (cmd == "POSITIVE")
                    {
                        // clearScreen();
                        cout << "Login Successful! Welcome, " << username << "!\n";
                        homepage_init(sockfd);
                        homepage_Menu(tempAccount);
                        return;
                    }
                }
                else if (choice == 2)
                {
                    forgotPassword(username);
                    cout << "Please login with your new password.\n";
                    break;
                }
                else
                {
                    cout << "Invalid option.\n";
                }
            }
        }
    }
}
void preloginScreens::user::signup()
{
    string name, username, password, favAni;
    Date DOB;
    
    name = acceptName();
    while(1)
    {
        username = acceptUsername();
        UserAccount account;
        account.setUsername(username);
        Packet p("UNIQUE-USERNAME-CHECK", account);
        p.display();
        p.write(sockfd);
        p.read(sockfd);


        if(p.getCommand() != "POSITIVE")
        {
            cout << "Username already taken\n";
            continue;
        }
        else
        {
            break;
        }
    }
    password = acceptPassword();
    DOB = acceptDOB();
    favAni = acceptFavAni();

    UserAccount user(name, username, password, DOB, 0, "", favAni);
    user.printAccountInfo(0);
    Packet packet("SIGNUP", user);
    packet.display();
    packet.write(sockfd);
}
void preloginScreens::admin::signup()
{
}
void preloginScreens::admin::login()
{
}



void preloginScreens::user::forgotPassword(string username)
{
    cout << " -------- FORGOT PASSWORD -------- \n";

    UserAccount tempAccount;
    tempAccount.setUsername(username);

    cout << "Security Question 1:\n";
    Date DOB = acceptDOB();

    cout << "Security Question 2:\n";
    string favAni = acceptFavAni();

    tempAccount.setDOB(DOB);
    tempAccount.setFavAni(favAni);

    Packet verifyPacket("FORGOT-PASS", tempAccount);
    verifyPacket.write(sockfd);

    Packet response;
    response.read(sockfd);

    if(response.getCommand() == "NEGATIVE")
    {
        clearScreen();
        cout << "Wrong answer. Returning to login.\n";
        return;
    }
    else
    {
        clearScreen();
        cout << "Identity verified!\n";
        cout << "Enter new password\n";
        string newPassword = acceptPassword();
    
        UserAccount changeAccount;
        changeAccount.setUsername(username);
        changeAccount.setPassword(newPassword);
        Packet changePacket("CHANGE-PASS", changeAccount);
        changePacket.write(sockfd);
    
        response.read(sockfd);
        if(response.getCommand() == "POSITIVE")
        {
            clearScreen();
            cout << "Password changed successfully! Please login with your new password.\n";
        }
        else
        {
            cout << "Something went wrong. Try again later.\n";
        }

    }


}