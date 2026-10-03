#include "EthernetFrame.h"
#include <iostream>

EthernetFrame::EthernetFrame(const std::string& sourceMAC,
                             const std::string& destinationMAC,
                             const Packet& packet)
    : sourceMAC(sourceMAC),
      destinationMAC(destinationMAC),
      packet(packet) {}

std::string EthernetFrame::getSourceMAC() const {
    return sourceMAC;
}

std::string EthernetFrame::getDestinationMAC() const {
    return destinationMAC;
}

Packet EthernetFrame::getPacket() const {
    return packet;
}

void EthernetFrame::display() const {
    std::cout << "\n===== ETHERNET FRAME =====\n";
    std::cout << "Source MAC      : " << sourceMAC << '\n';
    std::cout << "Destination MAC : " << destinationMAC << '\n';
    packet.display();
}