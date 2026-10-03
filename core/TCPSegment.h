#ifndef TCPSEGMENT_H
#define TCPSEGMENT_H
#include <string>
#include <cstdint>
enum class TCPFlag{NONE=0,SYN=1,ACK=2,FIN=4,RST=8};
class TCPSegment{
private:
    int sourcePort;
    int destinationPort;
    uint32_t sequenceNumber;
    uint32_t acknowledgmentNumber;
    uint16_t windowSize;
    uint8_t flags;
    std::string payload;
    unsigned short checksum;
public:
    TCPSegment(int sourcePort,int destinationPort,uint32_t sequenceNumber,uint32_t acknowledgmentNumber,uint16_t windowSize,uint8_t flags,const std::string& payload);
    int getSourcePort() const;
    int getDestinationPort() const;
    uint32_t getSequenceNumber() const;
    uint32_t getAcknowledgmentNumber() const;
    uint16_t getWindowSize() const;
    uint8_t getFlags() const;
    std::string getPayload() const;
    int getPayloadSize() const;
    unsigned short getChecksum() const;
    bool isValidPort() const;
    bool hasFlag(TCPFlag flag) const;
    void calculateChecksum();
    bool verifyChecksum() const;
    void display() const;
};
#endif