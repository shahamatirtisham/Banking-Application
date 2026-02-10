#include "pre-login.hpp"
#include <iostream>
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
    bool response = false;
    //while(!response)
    //{
        name = acceptName();
        

    //}
    username = acceptUsername();
    password = acceptPassword();
    DOB = acceptDOB();
    favAni = acceptFavAni();

    UserAccount user(name, username, password, DOB, 0, "", favAni);
    user.printAccountInfo(0);
    Packet packet(sockfd, "SIGNUP", user);
    packet.display();
    packet.write();


    // int n = write(sockfd, command, sizeof(command));
    // if(n < 0)
    //         error("Error on writing.");

    // n = read(sockfd, &account_no, sizeof(account_no));
    // if(n < 0)
    //         error("Error on writing.");

    //printf("Generated acc no\n");


    // package_command(command, "SIGNUP", username, password, date_of_birth, favourite_animal, account_no, salt);

    // write(sockfd, command, sizeof(command));

    // read(sockfd, &response, sizeof(response));
    // if(response == true)
    // {
    //     clear_screen();
    //     printf("Account creation successful. Your account number is %s\n\n", account_no);
    // }
    // else
    // {
    //     printf("Account creation failed.\n");
    // }
}
static void preloginScreens::admin::signup()
{

}
static void preloginScreens::admin::login()
{

}





