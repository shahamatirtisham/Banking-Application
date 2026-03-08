#pragma once
// #include <iostream>
// #include "../../shared/input.hpp"
// #include "../../shared/clearScreen.hpp"

namespace preloginScreens::user
{
    void signup();
    void login();
    void forgotPassword(std::string username);

}

namespace preloginScreens::admin
{
    void signup();
    void login();
}

void screens_init(int passed_sockfd);
void mainMenu();







