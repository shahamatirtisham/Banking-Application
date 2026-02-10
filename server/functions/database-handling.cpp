#include <iostream>
#include "../../shared/classes/UserAccount.hpp"
using namespace std;


class Database
{   
private:
    int portNo;
    UserAccount user;
    void database_init();
public:
    void addUser(UserAccount user);
    void deleteUser();

};



bool checkUniqueUsername(std::string username)
{
    // input dbms code
    return true;
}
void addUser(UserAccount user)
{
    // input dbms code
}