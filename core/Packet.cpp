#include "Packet.h"
#include <iostream>

Packet::Packet(int id, const IPv4Address& sourceIP,
               const IPv4Address& destinationIP,
               int size, const std::string& protocol, int ttl)
    : id(id), sourceIP(sourceIP), destinationIP(destinationIP),
      size(size), protocol(protocol), ttl(ttl) {}

int Packet::getId() const {
    return id;
}

IPv4Address Packet::getSourceIP() const {
    return sourceIP;
}

IPv4Address Packet::getDestinationIP() const {
    return destinationIP;
}

int Packet::getSize() const {
    return size;
}

std::string Packet::getProtocol() const {
    return protocol;
}

int Packet::getTTL() const {
    return ttl;
}

void Packet::decrementTTL() {
    if (ttl > 0)
        --ttl;
}

bool Packet::isExpired() const {
    return ttl <= 0;
}

void Packet::display() const {
    std::cout << "Packet #" << id
              << " | " << sourceIP.toString()
              << " -> " << destinationIP.toString()
              << " | Size: " << size << " bytes"
              << " | Protocol: " << protocol
              << " | TTL: " << ttl << '\n';
}