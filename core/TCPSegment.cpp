#include "TCPSegment.h"
#include <iostream>
TCPSegment::TCPSegment(int sourcePort,int destinationPort,uint32_t sequenceNumber,uint32_t acknowledgmentNumber,uint16_t windowSize,uint8_t flags,const std::string& payload):sourcePort(sourcePort),destinationPort(destinationPort),sequenceNumber(sequenceNumber),acknowledgmentNumber(acknowledgmentNumber),windowSize(windowSize),flags(flags),payload(payload),checksum(0){calculateChecksum();}
int TCPSegment::getSourcePort() const{return sourcePort;}
int TCPSegment::getDestinationPort() const{return destinationPort;}
uint32_t TCPSegment::getSequenceNumber() const{return sequenceNumber;}
uint32_t TCPSegment::getAcknowledgmentNumber() const{return acknowledgmentNumber;}
uint16_t TCPSegment::getWindowSize() const{return windowSize;}
uint8_t TCPSegment::getFlags() const{return flags;}
std::string TCPSegment::getPayload() const{return payload;}
int TCPSegment::getPayloadSize() const{return static_cast<int>(payload.size());}
unsigned short TCPSegment::getChecksum() const{return checksum;}
bool TCPSegment::isValidPort() const{return sourcePort>=0&&sourcePort<=65535&&destinationPort>=0&&destinationPort<=65535;}
bool TCPSegment::hasFlag(TCPFlag flag) const{return (flags&static_cast<uint8_t>(flag))!=0;}
void TCPSegment::calculateChecksum(){
    uint64_t value=static_cast<uint64_t>(sourcePort)+static_cast<uint64_t>(destinationPort)+sequenceNumber+acknowledgmentNumber+windowSize+flags+payload.size();
    for(unsigned char c:payload)value+=c;
    checksum=static_cast<unsigned short>(value&0xFFFF);
}
bool TCPSegment::verifyChecksum() const{
    uint64_t value=static_cast<uint64_t>(sourcePort)+static_cast<uint64_t>(destinationPort)+sequenceNumber+acknowledgmentNumber+windowSize+flags+payload.size();
    for(unsigned char c:payload)value+=c;
    return checksum==static_cast<unsigned short>(value&0xFFFF);
}
void TCPSegment::display() const{
    std::cout<<"\n===== TCP SEGMENT =====\n";
    std::cout<<"Source Port          : "<<sourcePort<<'\n';
    std::cout<<"Destination Port     : "<<destinationPort<<'\n';
    std::cout<<"Sequence Number      : "<<sequenceNumber<<'\n';
    std::cout<<"Acknowledgment Number: "<<acknowledgmentNumber<<'\n';
    std::cout<<"Window Size          : "<<windowSize<<'\n';
    std::cout<<"Flags                : ";
    if(flags==0)std::cout<<"NONE";
    else{
        if(hasFlag(TCPFlag::SYN))std::cout<<"SYN ";
        if(hasFlag(TCPFlag::ACK))std::cout<<"ACK ";
        if(hasFlag(TCPFlag::FIN))std::cout<<"FIN ";
        if(hasFlag(TCPFlag::RST))std::cout<<"RST ";
    }
    std::cout<<'\n';
    std::cout<<"Payload Size         : "<<payload.size()<<" bytes\n";
    std::cout<<"Payload              : "<<payload<<'\n';
    std::cout<<"Checksum             : "<<checksum<<'\n';
    std::cout<<"Checksum Valid       : "<<(verifyChecksum()?"YES":"NO")<<'\n';
}