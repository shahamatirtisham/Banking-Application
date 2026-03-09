#include "Colors.hpp"

// Color Codes
const string Color::RESET = "\033[0m";

const string Color::RED = "\033[31m";
const string Color::GREEN = "\033[32m";
const string Color::YELLOW = "\033[33m";
const string Color::BLUE = "\033[34m";
const string Color::MAGENTA = "\033[35m";
const string Color::CYAN = "\033[36m";

const string Color::BOLD = "\033[1m";
const string Color::UNDERLINE = "\033[4m";

string Color::last(const string &code, const string &text)
{
    return code + text + RESET;
}

string Color::red(const string &text) { return last(RED, text); }
string Color::green(const string &text) { return last(GREEN, text); }
string Color::yellow(const string &text) { return last(YELLOW, text); }
string Color::blue(const string &text) { return last(BLUE, text); }
string Color::magenta(const string &text) { return last(MAGENTA, text); }
string Color::cyan(const string &text) { return last(CYAN, text); }
string Color::bold(const string &text) { return last(BOLD, text); }
string Color::underline(const string &text) { return last(UNDERLINE, text); }