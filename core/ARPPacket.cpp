#include "ARPPacket.h"
#include <iostream>

ARPPacket::ARPPacket(ARPOperation operation,
                     const IPv4Address& senderIP,
                     const std::string& senderMAC,
                     const IPv4Address& targetIP,
                     const std::string& targetMAC)
    : operation(operation),
      senderIP(senderIP),
      senderMAC(senderMAC),
      targetIP(targetIP),
      targetMAC(targetMAC) {}

ARPOperation ARPPacket::getOperation() const {
    return operation;
}

IPv4Address ARPPacket::getSenderIP() const {
    return senderIP;
}

std::string ARPPacket::getSenderMAC() const {
    return senderMAC;
}

IPv4Address ARPPacket::getTargetIP() const {
    return targetIP;
}

std::string ARPPacket::getTargetMAC() const {
    return targetMAC;
}

void ARPPacket::display() const {
    std::cout << "\n===== ARP PACKET =====\n";

    if (operation == ARPOperation::REQUEST)
        std::cout << "Operation       : ARP REQUEST\n";
    else
        std::cout << "Operation       : ARP REPLY\n";

    std::cout << "Sender IP       : " << senderIP.toString() << '\n';
    std::cout << "Sender MAC      : " << senderMAC << '\n';
    std::cout << "Target IP       : " << targetIP.toString() << '\n';
    std::cout << "Target MAC      : " << targetMAC << '\n';
}