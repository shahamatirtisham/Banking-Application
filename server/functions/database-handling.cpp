#include <iostream>
#include "../../server/functions/database-handling.hpp"
#include "../../shared/classes/Packet.hpp"
#include "pre-login.hpp"
#include "../../shared/input.hpp"
#include "../../shared/classes/UserAccount.hpp"

static int sockfd = -1;

using namespace std;

bool checkUniqueUsername(std::string username)
{
    // input dbms code
}

static void forgetPassword(string username){
    clearScreen();
    cout << "--- Password Recovery ---\n";

    Date dob = acceptDOB();
    string favAni = acceptFavAni();

    Packet recoveryPacket(sockfd, "CHECK_RECOVERY", "", username, "", dob, 0, "", favAni);
    recoveryPacket.write();

    Packet response(sockfd);
    response.read();

    if (response.getCommand() == "TRUE")
    {
        cout << "Details Verified.\n";
        
        string newPass = acceptPassword();
        
        Packet updatePacket(sockfd, "RESET_PASS", "", username, newPass);
        updatePacket.write();
        
        Packet updateResponse(sockfd);
        updateResponse.read();
        
        if(updateResponse.getCommand() == "TRUE")
        {
            cout << "Password Reset Successfully!\n";
        }
        else
        {
            cout << "Error resetting password.\n";
        }
    }
    else
    {
        cout << "Recovery details did not match our records.\n";
        cout << "Press Enter to continue ... ";
        cin.get();
    }
}

