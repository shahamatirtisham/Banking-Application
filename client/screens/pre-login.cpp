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
        TextBox tb_welcome("WELCOME TO AOZORA BANK", 40);
                
        vector<string> lines(2);
        lines[0] = "1. Login as an existing user";
        lines[1] = "2. Signup as a new user";

        TextBox options(lines, 40, false, 3);

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
        TextBox tb_login("LOGIN", 40);

        cout << "▶ Enter your username: ";
        username = acceptUsername();

        cout << "▶ Enter your password: ";
        password = acceptPassword();

        // Build and send login packet
        UserAccount tempAccount;
        tempAccount.setUsername(username);
        tempAccount.setPassword(password);

        // cout << "temp acc loaded\n";

        Packet loginPacket("LOGIN", tempAccount);
        // cout << "packet loaded\n";
        loginPacket.write(sockfd);
        // cout << "packet written\n";

        // Read server response
        Packet response;
        response.read(sockfd);

        string cmd = response.getCommand();

        if (cmd == "POSITIVE")
        {
            cout << "✔ Login Successful! Loading homepage...\n";
            sleep(1000);
            clearScreen();
            homepage_init(sockfd);
            homepage_Menu(tempAccount);
            return;
        }
        else if (cmd == "NEGATIVE")
        {
            while (true)
            {
                clearScreen();
                vector<string> strs(4);
                strs[0] = "Username and password combination doesn't exist :(";
                strs[1] = "1. Retry password";
                strs[2] = "2. Forgot password";
                strs[3] = "0. Back to main menu";
                TextBox tb_forgotPassword(strs, 40, false, 3);

                cout << "▶ Select: ";
                int choice = readInt();

                if (choice == 0)
                    return;

                else if (choice == 1)
                {
                    cout << "▶ Enter your password: ";
                    password = acceptPassword();
                    tempAccount.setPassword(password);
                    Packet retryPacket("LOGIN", tempAccount);
                    retryPacket.write(sockfd);

                    response.read(sockfd);
                    cmd = response.getCommand();
                    

                    if (cmd == "POSITIVE")
                    {
                        cout << "✔ Login Successful! Loading homepage...\n";
                        sleep(1000);
                        clearScreen();
                        homepage_init(sockfd);
                        homepage_Menu(tempAccount);
                        return;
                    }
                }
                else if (choice == 2)
                {
                    forgotPassword(username);
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
    TextBox signup("SIGNUP", 40);


    cout << "▶ Set name: ";
    name = acceptName();

    while(1)
    {
        cout << "▶ Set username: ";
        username = acceptUsername();
        UserAccount account;
        account.setUsername(username);
        Packet p("UNIQUE-USERNAME-CHECK", account);
        // p.display();
        p.write(sockfd);
        p.read(sockfd);


        if(p.getCommand() != "POSITIVE")
        {
            cout << "✖ Username already taken, try a different one\n";
            continue;
        }
        else
        {
            break;
        }
    }
    cout << "▶ Set password: ";
    password = acceptPassword();
    cout << "▶ Set date of birth (DD-MM-YYY): ";
    DOB = acceptDOB();
    cout << "▶ Set favorite animal: ";
    favAni = acceptFavAni();

    UserAccount user(name, username, password, DOB, 0, "", favAni);
    user.printAccountInfo(0);
    Packet packet("SIGNUP", user);
    // packet.display();
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

    // TextBox tb_forgotPass("RESET PASSWORD", 40);

    UserAccount tempAccount;
    tempAccount.setUsername(username);

    cout << "▶ Security Question 1, enter your date of birth: ";
    Date DOB = acceptDOB();

    cout << "▶ Security Question 1, enter your favorite animal: ";
    string favAni = acceptFavAni();

    tempAccount.setDOB(DOB);
    tempAccount.setFavAni(favAni);

    Packet verifyPacket("FORGOT-PASS", tempAccount);
    verifyPacket.write(sockfd);

    Packet response;
    response.read(sockfd);

    if(response.getCommand() == "NEGATIVE")
    {
        cout << "✖ Login failed, returning to homescreen...";
        sleep(1000);
        clearScreen();
        return;
    }
    else
    {
        cout << "✔ Identity verified! Login with your new password\n";
        sleep(2000);
        clearScreen();

        TextBox resetPass("RESET PASSWORD", 40);
        
        cout << "▶ Enter new password: ";
        string newPassword = acceptPassword();
    
        UserAccount changeAccount;
        changeAccount.setUsername(username);
        changeAccount.setPassword(newPassword);
        Packet changePacket("CHANGE-PASS", changeAccount);
        changePacket.write(sockfd);
    
        response.read(sockfd);
        if(response.getCommand() == "POSITIVE")
        {
            cout << "✔ Password changed successfully! Please login with your new password.\n";
            sleep(1000);
            clearScreen();
        }
        else
        {
            cout << "✖ Something went wrong. Try again later.\n";
        }

    }


}