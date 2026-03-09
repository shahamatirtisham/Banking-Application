#include <iostream>
#include <string>
#include "Hexadecimal.hpp"
using namespace std;

Hexadecimal::Hexadecimal(int decimalVal)
{
    setValue(decimalVal);
}
Hexadecimal::Hexadecimal(const string& hex_str)
{
    int decimalVal = stoll(hex_str, nullptr, 16);
    setValue(decimalVal);
}
string Hexadecimal::decToHex(int decimalVal) const
{
    if(decimalVal == 0)
        return "0";

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

// int main()
// {
//     for(int i = 0; i < 100; i++)
//     {
//         Hexadecimal hex(i);
//         cout << i << ": " << hex << endl;
//     }





//     return 0;
// }

