#ifndef PACKET_H
#define PACKET_H

#include "IPv4Address.h"
#include <string>

class Packet {
private:
    int id;
    IPv4Address sourceIP;
    IPv4Address destinationIP;
    int size;
    std::string protocol;
    int ttl;

public:
    Packet(int id, const IPv4Address& sourceIP,
           const IPv4Address& destinationIP,
           int size, const std::string& protocol, int ttl = 64);

    int getId() const;
    IPv4Address getSourceIP() const;
    IPv4Address getDestinationIP() const;
    int getSize() const;
    std::string getProtocol() const;
    int getTTL() const;

    void decrementTTL();
    bool isExpired() const;

    void display() const;
};
#endif