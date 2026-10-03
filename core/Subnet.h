#ifndef SUBNET_H
#define SUBNET_H

#include "IPv4Address.h"

class Subnet {
private:
    IPv4Address ip;
    int prefixLength;

public:
    Subnet(
        const IPv4Address& ip,
        int prefixLength);

    unsigned int getMask() const;

    IPv4Address getNetworkAddress() const;
    IPv4Address getBroadcastAddress() const;
    IPv4Address getFirstHost() const;
    IPv4Address getLastHost() const;

    unsigned int getHostCount() const;

    bool contains(
        const IPv4Address& address) const;

    bool sameSubnet(
        const IPv4Address& address) const;

    std::string toString() const;

    void display() const;
};

#endif