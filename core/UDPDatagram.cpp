#include "UDPDatagram.h"
#include <iostream>
UDPDatagram::UDPDatagram(int sourcePort,int destinationPort,const std::string& payload):sourcePort(sourcePort),destinationPort(destinationPort),payload(payload),checksum(0){calculateChecksum();}
int UDPDatagram::getSourcePort() const{return sourcePort;}
int UDPDatagram::getDestinationPort() const{return destinationPort;}
std::string UDPDatagram::getPayload() const{return payload;}
int UDPDatagram::getPayloadSize() const{return static_cast<int>(payload.size());}
unsigned short UDPDatagram::getChecksum() const{return checksum;}
bool UDPDatagram::isValidPort() const{return sourcePort>=0&&sourcePort<=65535&&destinationPort>=0&&destinationPort<=65535;}
void UDPDatagram::calculateChecksum(){
    unsigned int value=0;
    value+=static_cast<unsigned int>(sourcePort);
    value+=static_cast<unsigned int>(destinationPort);
    value+=static_cast<unsigned int>(payload.size());
    for(unsigned char c:payload)value+=c;
    checksum=static_cast<unsigned short>(value&0xFFFF);
}
bool UDPDatagram::verifyChecksum() const{
    unsigned int value=0;
    value+=static_cast<unsigned int>(sourcePort);
    value+=static_cast<unsigned int>(destinationPort);
    value+=static_cast<unsigned int>(payload.size());
    for(unsigned char c:payload)value+=c;
    return checksum==static_cast<unsigned short>(value&0xFFFF);
}
void UDPDatagram::display() const{
    std::cout<<"\n===== UDP DATAGRAM =====\n";
    std::cout<<"Source Port      : "<<sourcePort<<'\n';
    std::cout<<"Destination Port : "<<destinationPort<<'\n';
    std::cout<<"Payload Size     : "<<payload.size()<<" bytes\n";
    std::cout<<"Payload          : "<<payload<<'\n';
    std::cout<<"Checksum         : "<<checksum<<'\n';
    std::cout<<"Checksum Valid   : "<<(verifyChecksum()?"YES":"NO")<<'\n';
}