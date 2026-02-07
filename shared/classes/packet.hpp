#include <iostream>
#include <unistd.h>
#include <cstring>

using namespace std;

class Packet
{
private:
    int clientSock;
    string seg1;
    string seg2;
    string seg3;
    string seg4;
    string seg5;
    string seg6;
    string seg7;
    string seg8;
public:
    Packet(int clientSock, string seg1="", string seg2="", string seg3="",
           string seg4="", string seg5="", string seg6="", string seg7="",
           string seg8="")
    {
        this->clientSock = clientSock;
        this->seg1 = "";
        this->seg2 = "";
        this->seg3 = "";
        this->seg4 = "";
        this->seg5 = "";
        this->seg6 = "";
        this->seg7 = "";
        this->seg8 = "";
        
        this->seg1 = seg1;
        this->seg2 = seg2;
        this->seg3 = seg3;
        this->seg4 = seg4;
        this->seg5 = seg5;
        this->seg6 = seg6;
        this->seg7 = seg7;
        this->seg8 = seg8;
    }
    string getSeg1() const;
    string getSeg2() const;
    string getSeg3() const;
    string getSeg4() const;
    string getSeg5() const;
    string getSeg6() const;
    string getSeg7() const;
    string getSeg8() const;
    void packetWrite();
    void packetRead();
};
string Packet::getSeg1() const
{
    return seg1;
}
string Packet::getSeg2() const
{
    return seg2;
}
string Packet::getSeg3() const
{
    return seg3;
}
string Packet::getSeg4() const
{
    return seg4;
}
string Packet::getSeg5() const
{
    return seg5;
}
string Packet::getSeg6() const
{
    return seg6;
}
string Packet::getSeg7() const
{
    return seg7;
}
string Packet::getSeg8() const
{
    return seg8;
}
void Packet:: packetWrite()
{
    write(clientSock, seg1.c_str(), 65);
    write(clientSock, seg2.c_str(), 65);
    write(clientSock, seg3.c_str(), 65);
    write(clientSock, seg4.c_str(), 65);
    write(clientSock, seg5.c_str(), 65);
    write(clientSock, seg6.c_str(), 65);
    write(clientSock, seg7.c_str(), 65);
    write(clientSock, seg8.c_str(), 65);
}
void Packet:: packetRead()
{
    seg1.resize(65);
    seg2.resize(65);
    seg3.resize(65);
    seg4.resize(65);
    seg5.resize(65);
    seg6.resize(65);
    seg7.resize(65);
    seg8.resize(65);

    read(clientSock, seg1.data(), 65);
    read(clientSock, seg2.data(), 65);
    read(clientSock, seg3.data(), 65);
    read(clientSock, seg4.data(), 65);
    read(clientSock, seg5.data(), 65);
    read(clientSock, seg6.data(), 65);
    read(clientSock, seg7.data(), 65);
    read(clientSock, seg8.data(), 65);
}



