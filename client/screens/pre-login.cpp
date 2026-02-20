#include <iostream>
#include "pre-login.hpp"
#include "../../shared/input.hpp"
#include "../../shared/classes/Packet.hpp"
#include "../../shared/classes/UserAccount.hpp"
#include "../../shared/hash.hpp"
#include "../../server/functions/database-handling.hpp"

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
    clearScreen();
    cout << "Welcome to Londu Bank!\n\n";

    while (1)
    {
        cout << "1. Login as existing user.\n"
                "2. Signup as new user.\n"
                "3. Register as an admin.\n"
                "4. Login as admin.\n"
                "Please select your desired option (1-4): ";
        int choice = readInt();

        switch (choice)
        {
        case 1:
            clearScreen();
            preloginScreens::user::login();
            break;

        case 2:
            clearScreen();
            preloginScreens::user::signup();
            break;

        case 3:
            clearScreen();
            preloginScreens::admin::signup();
            break;

        case 4:
            clearScreen();
            preloginScreens::admin::login();
            break;

        default:
            clearScreen();
            cout << "Invalid choice. Try again.\n";
        }
    }
}

static void preloginScreens::user::login()
{
    string username, password;

    while (true)
    {
        clearScreen();
        cout << " -------- LOGIN -------- \n";

        cout << "Enter your username: ";
        username = acceptUsername();

        cout << "Enter your password: ";
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

        if (cmd == "LOGIN-SUCCESS")
        {
            clearScreen();
            cout << "Login Successful! Welcome, " << username << "!\n";
            // TODO: abtahi's post-login screen goes here
            return;
        }
        else if (cmd == "USER-NOT-FOUND")
        {
            clearScreen();
            cout << "Username \"" << username << "\" not found. Try again.\n";
            continue;
        }
        else if (cmd == "LOGIN-FAIL")
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
                    cout << "Enter password: ";
                    password = acceptPassword();

                    tempAccount.setPassword(password);
                    Packet retryPacket("LOGIN", tempAccount);
                    retryPacket.write(sockfd);

                    response.read(sockfd);

                    if (response.getCommand() == "LOGIN-SUCCESS")
                    {
                        clearScreen();
                        cout << "Login Successful! Welcome, " << username << "!\n";
                        // TODO: abtahi er post login
                        return;
                    }
                    else
                    {
                        cout << "Wrong password again.\n";
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
static void preloginScreens::user::signup()
{
    string name, username, password, favAni;
    Date DOB;
    // Packet packet(sockfd);
    name = acceptName();
    while (1)
    {
        username = acceptUsername();
        UserAccount account;
        account.setUsername(username);
        Packet p("UNIQUE-USERNAME-CHECK", account);
        p.display();
        p.write(sockfd);
        p.read(sockfd);
        // p.display();

        if (p.getCommand() != "POSITIVE")
        {
            cout << "Username already taken\n";
            continue;
        }
        else
        {
            // cout << "Username is unique. Moving on\n";
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
static void preloginScreens::admin::signup()
{
}
static void preloginScreens::admin::login()
{
}



static void forgotPassword(string username)
{
    clearScreen();
    cout << " -------- FORGOT PASSWORD -------- \n";

    UserAccount tempAccount;
    tempAccount.setUsername(username);
    Packet requestPacket("FORGOT-PASS", tempAccount);
    requestPacket.write(sockfd);

    Packet response;
    response.read(sockfd);

    if(response.getCommand() == "USER-NOT-FOUND")
    {
        cout << "Username not found.\n";
        return;
    }

    cout << "Security Question: What is your favourite animal?\n";
    cout << "Answer: ";
    string favAni;
    cin >> favAni;

    UserAccount answerAccount;
    answerAccount.setUsername(username);
    answerAccount.setFavAni(favAni);
    Packet verifyPacket("VERIFY-FAVANI", answerAccount);
    verifyPacket.write(sockfd);

    response.read(sockfd);

    if(response.getCommand() == "WRONG-ANSWER")
    {
        clearScreen();
        cout << "Wrong answer. Returning to login.\n";
        return;
    }

    clearScreen();
    cout << "Identity verified!\n";
    cout << "Enter new password: ";
    string newPassword = acceptPassword();

    UserAccount changeAccount;
    changeAccount.setUsername(username);
    changeAccount.setPassword(newPassword);
    Packet changePacket("CHANGE-PASS", changeAccount);
    changePacket.write(sockfd);

    response.read(sockfd);

    if(response.getCommand() == "PASS-CHANGED")
    {
        clearScreen();
        cout << "Password changed successfully! Please login with your new password.\n";
    }
    else
    {
        cout << "Something went wrong. Try again later.\n";
    }
}