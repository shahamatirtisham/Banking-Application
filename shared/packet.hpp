#pragma once

#include <iostream>

using namespace std;

class Packet
{
private:
    int clientSock;
    char seg1[20];
    char seg2[20];
    char seg3[20];
public:
    Packet(int clientSock, const char seg1[]={0}, const char seg2[]={0}, const char seg3[]={0});
    string getSeg1();
    string getSeg2();
    string getSeg3();
    void packetFill(const char seg1[]={0}, const char seg2[]={0}, const char seg3[]={0});
    void packetWrite();
    void packetRead();
};