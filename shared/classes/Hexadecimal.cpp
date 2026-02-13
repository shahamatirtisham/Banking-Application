#include <iostream>
#include "Hexadecimal.hpp"
using namespace std;

Hexadecimal::Hexadecimal(int decimalVal)
{
    setValue(decimalVal);
}
string Hexadecimal::decToHex(int decimalVal) const
{
    char hexSymbols[17] = {"0123456789ABCDEF"};
    string hexVal = "";
    while(decimalVal > 0)
    {
        hexVal = hexSymbols[decimalVal % 16] + hexVal;
        decimalVal /= 16; 
    }
    // cout << hexVal;
    return hexVal;
    
}
void Hexadecimal::setValue(int decimalVal)
{
    this->decimalVal = decimalVal;
    this->hexadecimalVal = decToHex(decimalVal);
}
int Hexadecimal::getDecimal() const
{
    return decimalVal;
}
string Hexadecimal::getHexadecimalVal() const
{
    return hexadecimalVal;
}
ostream& operator << (std::ostream& COUT, const Hexadecimal& hexadecimal)
{
    COUT << hexadecimal.getHexadecimalVal();
    return COUT;
}
Hexadecimal& Hexadecimal::operator = (const Hexadecimal& other)
{
    this->decimalVal = other.decimalVal;
    this->hexadecimalVal = other.hexadecimalVal;
    return *this;
}

