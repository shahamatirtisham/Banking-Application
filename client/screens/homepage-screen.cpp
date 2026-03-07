#include<iostream>
#include"homepage-screen.hpp"
#include"HomePage.hpp"
#include"../classes/user-operations.hpp"

static int sockfd = -1;

void homepage_init(int passed_sockfd)
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

void homepage_Menu(UserAccount user)
{
    useroperation_init(sockfd);
    cout <<"=========== sockfd " <<sockfd;

    HomePage hp(user);
    
    hp.display();
    int choice = hp.getChoice();
    hp.userOperation(choice);
}