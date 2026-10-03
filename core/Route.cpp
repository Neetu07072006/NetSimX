#include "Route.h"
#include <iostream>

Route::Route(const IPv4Address& network,
             int prefixLength,
             const IPv4Address& nextHop,
             const std::string& interfaceName,
             int metric)
    : network(network),
      prefixLength(prefixLength),
      nextHop(nextHop),
      interfaceName(interfaceName),
      metric(metric) {}

bool Route::matches(
    const IPv4Address& destination) const {

    if (prefixLength == 0)
        return true;

    unsigned int mask =
        0xFFFFFFFFu << (32 - prefixLength);

    return (network.toInteger() & mask) ==
           (destination.toInteger() & mask);
}

int Route::getPrefixLength() const {
    return prefixLength;
}

IPv4Address Route::getNetwork() const {
    return network;
}

IPv4Address Route::getNextHop() const {
    return nextHop;
}

std::string Route::getInterfaceName() const {
    return interfaceName;
}

int Route::getMetric() const {
    return metric;
}

void Route::display() const {
    std::cout << network.toString()
              << "/"
              << prefixLength
              << "\t";

    if (nextHop.isValid())
        std::cout << nextHop.toString();
    else
        std::cout << "Direct";

    std::cout << "\t"
              << interfaceName
              << "\t"
              << metric
              << '\n';
}