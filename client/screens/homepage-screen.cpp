#include<iostream>
#include"homepage-screen.hpp"
#include"HomePage.hpp"
#include"../classes/user-operations.hpp"

static int sockfd = -1;

void homepage_init(int passed_sockfd)
{
    if (passed_sockfd >= 0)
    {
        cout << "homepage initialized\n";
        sockfd = passed_sockfd;
    }
    else
    {
        perror("sockfd not intialized\n");
    }
}

void homepage_Menu(const UserAccount& user)
{
    cout <<"sockfd in homepage: " <<sockfd << endl;
    // useroperation_init(sockfd);

    HomePage hp(sockfd, user);
    
    int choice = 0;

    while(choice != 6)
    {
        hp.display();
        choice = hp.getChoice();
        hp.userOperation(choice);
    }
}