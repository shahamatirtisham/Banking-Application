#include "TextBox.hpp"
// #include "../../shared/input.hpp"
#include <iostream>
#include <thread>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <vector>
using namespace std;

string TextBox::centre(const string &text, int width) const
{
    int padding = width - text.size();

    if (padding <= 0)
        return text;

    int left = padding / 2;
    int right = padding - left;

    return string(left, ' ') + text + string(right, ' ');
}
string TextBox::constructLine(const string &ch, int width) const
{
    string str = "";
    for (int i = 0; i < width; i++)
    {
        str += ch;
    }
    return str;
}

TextBox::TextBox(const vector<string> &lines, int width, bool center, int padding)
    : lines(lines), padding(padding), center(center)
{
   
    int maxTextWidth = 0;
    for (auto line : lines)
    {
        maxTextWidth = max(maxTextWidth, static_cast<int>(line.size()));
    }    
    if(width == 0)
    {
        this->width = maxTextWidth + 10;
    }
    else
    {
        if(width < maxTextWidth)
        {
            this->width = maxTextWidth + 10;
        }
        else
        {
            this->width = width;
        }
    }
    
    cout << *this;
}
TextBox::TextBox(const string &line, int width, bool center, int padding)
    : padding(padding), center(center)
{
    lines.assign(1, line);

    if (width == 0)
        this->width = line.size() + 10;
    else
        this->width = width;

    cout << *this;
}
string TextBox::generateBox() const
{
    // cout << "printing box with width: " << this->width << endl;
    stringstream ss;
    // cout << "step 1\n";
    ss << "╭" << constructLine("─", width) << "╮" << endl;
    // cout << "step 1\n";
    if (center)
    {
        for (auto line : lines)
        {
            // cout << "loop\n";
            ss << "│" << centre(line, width) << "│" << endl;
        }
    }
    else
    {
        for (auto line : lines)
        {
            // cout << "loop\n";
            ss << "│" << string(padding, ' ') << line << string(width - padding - line.size(), ' ') << "│" << endl;
        }
    }
    // cout << "quit loop\n";
    ss << "╰" << constructLine("─", width) << "╯" << endl;

    return ss.str();
}

ostream& operator << (ostream& out, const TextBox& tb)
{
    out << tb.generateBox();
    return out;
}



// ╭───────────────────────────────────╮
// │                                   │
// ╰───────────────────────────────────╯
// ─ │ ┌ ┐ └ ┘
// █ ▓ ▒ 
// > ▶
// ✔ ✖
// ● ○
// ■ □
// ▲ ▼

// void sleep(int ms)
// {
//     std::this_thread::sleep_for(std::chrono::milliseconds(ms));
// }

// int main()
// {cout << "Please login with your new password.\n";
   
    // TextBox welcomeBox("WELCOME TO LONDU BANK", 40);
    // sleep(500);
    // // cout << "constructor\n";
    // // cout << welcomeBox.generateBox();
    // // cout << "genned one\n";
    // vector<string> lines(2);
    // lines[0] = "1. Login as an existing user";
    // lines[1] = "2. Signup as a new user";
    // TextBox options(lines, 40, false, 3);
    // // cout << options.generateBox();

    // TextBox signup("SIGNUP", 40);
    // cout << signup;

    // int choice;
    // cout << "| ▶ Enter username: ";
    // cin >> choice;
    // sleep(2000);

    // cout << "✔ Login Success\n";
    // cout << "✖ Login Failed\n";

    // clearScreen();
    // vector<string> strs(4);
    // strs[0] = "Username and password combination doesn't exist :(";
    // strs[1] = "1. Retry password";
    // strs[2] = "2. Forgot password";
    // strs[3] = "0. Back to main menu";
    // TextBox tb_forgotPassword(strs, 40, false, 3);

    // return 0;
// }