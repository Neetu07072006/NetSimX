#ifndef ETHERNETFRAME_H
#define ETHERNETFRAME_H

#include "Packet.h"
#include <string>

class EthernetFrame {
private:
    std::string sourceMAC;
    std::string destinationMAC;
    Packet packet;

public:
    EthernetFrame(const std::string& sourceMAC,
                  const std::string& destinationMAC,
                  const Packet& packet);

    std::string getSourceMAC() const;
    std::string getDestinationMAC() const;
    Packet getPacket() const;

    void display() const;
};

#endif