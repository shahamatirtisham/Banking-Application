#ifndef COLOR_HPP
#define COLOR_HPP

#include <string>
using namespace std;

class Color {

private:
    static const string RESET;

    static const string RED;
    static const string GREEN;
    static const string YELLOW;
    static const string BLUE;
    static const string MAGENTA;
    static const string CYAN;
    // Formatter
    static const string BOLD;
    static const string UNDERLINE;

    static string last(const string& code, const string& text);

public:
    //colors
    static string red(const string& text);
    static string green(const string& text);
    static string yellow(const string& text);
    static string blue(const string& text);
    static string magenta(const string& text);
    static string cyan(const string& text);
    //formatting
    static string bold(const string& text);
    static string underline(const string& text);
};

#endif