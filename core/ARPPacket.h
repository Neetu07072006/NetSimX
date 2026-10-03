#ifndef ARPPACKET_H
#define ARPPACKET_H

#include "IPv4Address.h"
#include <string>

enum class ARPOperation {
    REQUEST,
    REPLY
};

class ARPPacket {
private:
    ARPOperation operation;
    IPv4Address senderIP;
    std::string senderMAC;
    IPv4Address targetIP;
    std::string targetMAC;

public:
    ARPPacket(ARPOperation operation,
              const IPv4Address& senderIP,
              const std::string& senderMAC,
              const IPv4Address& targetIP,
              const std::string& targetMAC);

    ARPOperation getOperation() const;
    IPv4Address getSenderIP() const;
    std::string getSenderMAC() const;
    IPv4Address getTargetIP() const;
    std::string getTargetMAC() const;

    void display() const;
};

#endif