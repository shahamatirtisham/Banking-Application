#include "pre-login.hpp"
#include <iostream>
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
    string username, password;

    while(true){
        cout << " ------------------------ LOGIN  ------------------------ ";

        cout << "Enter your username: " << endl;
        username = acceptUsername();

        cout << "Enter your password: " << endl;
        password = acceptPassword();

        Packet userCheckPacket(sockfd, "CHECK-user", "", username);
        userCheckPacket.write();

        Packet response(sockfd);
        response.read();

        if(response.getCommand() != "False"){
            clearScreen();
            cout << "Username not found! Please try again.\n";
            continue;
        }

        Packet passCheckPacket(sockfd, "CHECK-user", "", username, password);
        passCheckPacket.write();

        response.read();

        if(response.getCommand() == "TRUE"){
            clearScreen();
            cout << "Login Successful! Welcome " << username << endl;
            
            // Abtahi er kaam


            break;
        }

        else{
            
            bool loginSuccess = false;

            while(!loginSuccess){

                clearScreen();

                cout << "Incorrect Password for user: " << username << "\n";
                cout << "1. Retry Password\n";
                cout << "2. Forgot Password\n";
                cout << "0. Go Back to Main Menu\n";

                cout << "Select option: ";
                
                int choice = readInt();

                if (choice == 0) return;
                
                if (choice == 1)
                {
                    password = acceptPassword(); 
                    
                    Packet retryPacket(sockfd, "CHECK_PASS", "", username, password);
                    retryPacket.write();
                    
                    response.read();
                    
                    if (response.getCommand() == "TRUE")
                    {
                        cout << "Login Successful!\n";
                        loginSuccess = true;


                        // abtahi er kaam

                        return;
                    }
                }
                else if (choice == 2)
                {
                    forgotPassword(username);
                    // After changing password, we go back to the retry loop 
                    // or you can return to main menu. 
                    // For now, let's just break the retry loop and ask them to login again.
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

    name = acceptName();
    username = acceptUsername();
    password = acceptPassword();
    DOB = acceptDOB();
    favAni = acceptFavAni();

    UserAccount user(name, username, password, DOB, 0, favAni);
    user.printAccountInfo(0);
    Packet packet(sockfd, "SIGNUP", name, username, password, DOB, 0, "", favAni);
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





