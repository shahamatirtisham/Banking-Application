#pragma once
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
using namespace std;




class TextBox
{
    int width;
    int padding;
    bool center;
    vector<string> lines;
    string centre(const string& text, int width) const;
    string constructLine(const string& ch, int width) const;
public:
    TextBox(const vector<string>& lines, int width=0, bool center=true, int padding=0);
    TextBox(const string& line, int width=0, bool center=true, int padding=0);
    string generateBox() const;
};



// ╭───────────────────────────────────╮
// │                                   │ 
// ╰───────────────────────────────────╯

