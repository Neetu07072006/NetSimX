#ifndef UDPDATAGRAM_H
#define UDPDATAGRAM_H
#include <string>
class UDPDatagram{
private:
    int sourcePort;
    int destinationPort;
    std::string payload;
    unsigned short checksum;
public:
    UDPDatagram(int sourcePort,int destinationPort,const std::string& payload);
    int getSourcePort() const;
    int getDestinationPort() const;
    std::string getPayload() const;
    int getPayloadSize() const;
    unsigned short getChecksum() const;
    bool isValidPort() const;
    void calculateChecksum();
    bool verifyChecksum() const;
    void display() const;
};
#endif