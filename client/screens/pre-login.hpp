#pragma once
// #include <iostream>
// #include "../../shared/input.hpp"
// #include "../../shared/clearScreen.hpp"

namespace preloginScreens::user
{
    static void signup();
    static void login();
    static void forgotPassword(std::string username);

}

namespace preloginScreens::admin
{
    static void signup();
    static void login();
}

void screens_init(int passed_sockfd);
void mainMenu();







