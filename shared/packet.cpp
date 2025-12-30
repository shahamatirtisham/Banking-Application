#include "packet.hpp"
//#include <sys/file.h>
#include <unistd.h>
//#include <sys/types.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
#include <cstring>

using namespace std;

Packet::Packet(int clientSock, const char seg1[], const char seg2[], const char seg3[]) : clientSock(clientSock) 
{
    memset(this->seg1, 0, 20);
    memset(this->seg2, 0, 20);
    memset(this->seg3, 0, 20);

    strcpy(this->seg1, seg1);
    strcpy(this->seg2, seg2);
    strcpy(this->seg3, seg3);
}

string Packet::getSeg1()
{
    return string(seg1);
}
string Packet::getSeg2()
{
    return string(seg2);
}
void Packet:: packetWrite()
{
    write(clientSock, seg1, 20);
    write(clientSock, seg2, 20);
    write(clientSock, seg3, 20);
}
void Packet:: packetRead()
{
    cout << "Tryna read\n";
    read(clientSock, seg1, 20);
    read(clientSock, seg2, 20);
    read(clientSock, seg3, 20);
    cout << "Tryna reading done\n";
}
void Packet::packetFill(const char seg1[], const char seg2[], const char seg3[])
{
    memset(this->seg1, 0, 20);
    memset(this->seg2, 0, 20);
    memset(this->seg3, 0, 20);

    strcpy(this->seg1, seg1);
    strcpy(this->seg2, seg2);
    strcpy(this->seg3, seg3);
}


