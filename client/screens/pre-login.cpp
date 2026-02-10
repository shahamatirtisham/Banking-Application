#include <iostream>
#include "pre-login.hpp"
#include "../../shared/input.hpp"
#include "../../shared/classes/Packet.hpp"
#include "../../shared/classes/UserAccount.hpp"
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
    if(passed_sockfd >= 0)
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

    while(1)
    {
        cout << "1. Login as existing user.\n"
               "2. Signup as new user.\n"
               "3. Register as an admin.\n"
               "4. Login as admin.\n"
               "Please select your desired option (1-4): ";
        int choice = readInt();

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

static void preloginScreens::user::login()
{

}
static void preloginScreens::user::signup()
{
    string name, username, password, favAni;
    Date DOB;
    // Packet packet(sockfd);
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
        // p.display();

        if(p.getCommand() != "POSITIVE")
        {
            cout << "Username already taken\n";
            continue;
        }
        else
        {
            //cout << "Username is unique. Moving on\n";
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





