#pragma once
#include <iostream>

class Hexadecimal
{
private:
    int decimalVal;
    std::string hexadecimalVal;
    
public:
    Hexadecimal(int decimalVal);
    std::string decToHex(int decimal) const;
    void setValue(int decimalVal);
    int getDecimal() const;
    std::string getHexadecimalVal() const;
};
std::ostream& operator << (std::ostream& COUT, const Hexadecimal& hexadecimal);

