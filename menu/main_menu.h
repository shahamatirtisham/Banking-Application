#ifndef MAINMENU_H
#define MAINMENU_H

class MainMenu 
{
    private:
    void printHeader();
    void printMenu();
    int readChoice();
    
    public:
    void display();
    int getChoice();
};

#endif