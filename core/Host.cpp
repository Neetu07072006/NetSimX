#include "Host.h"
#include <iostream>

Host::Host(int id,
           const std::string& name,
           const std::string& macAddress)
    : NetworkNode(id, name, NodeType::HOST),
      macAddress(macAddress) {}

void Host::setMACAddress(const std::string& mac) {
    macAddress = mac;
}

std::string Host::getMACAddress() const {
    return macAddress;
}

ARPCache& Host::getARPCache() {
    return arpCache;
}

void Host::display() const {
    std::cout << "Host | ID: " << id
              << " | Name: " << name
              << " | IP: " << ipAddress.toString()
              << " | MAC: " << macAddress
              << " | Links: " << links.size()
              << '\n';
}