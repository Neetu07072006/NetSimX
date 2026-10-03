#ifndef ROUTE_H
#define ROUTE_H

#include "IPv4Address.h"
#include <string>

class Route {
private:
    IPv4Address network;
    int prefixLength;
    IPv4Address nextHop;
    std::string interfaceName;
    int metric;

public:
    Route(const IPv4Address& network,
          int prefixLength,
          const IPv4Address& nextHop,
          const std::string& interfaceName,
          int metric = 1);

    bool matches(const IPv4Address& destination) const;
    int getPrefixLength() const;
    IPv4Address getNetwork() const;
    IPv4Address getNextHop() const;
    std::string getInterfaceName() const;
    int getMetric() const;

    void display() const;
};

#endif