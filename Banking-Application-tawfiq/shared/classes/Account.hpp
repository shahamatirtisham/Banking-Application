#include <iostream>
using namespace std;

class Account
{
protected:
    string name;
    string username;
    string password;
    
public:
    Account(string name, string username, string password);
    Account();
    void setName(string name="");
    void setUsername(string username="");
    void setPassword(string password="");
    string getUsername() const;
    string getPassword() const;
    string getName() const;
    virtual void printAccountInfo(bool printAsterisk) const = 0;
};    


