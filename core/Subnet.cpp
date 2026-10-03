#include "Subnet.h"
#include <iostream>

Subnet::Subnet(
    const IPv4Address& ip,
    int prefixLength)
    : ip(ip),
      prefixLength(prefixLength) {}

unsigned int Subnet::getMask() const {

    if (prefixLength == 0)
        return 0;

    return 0xFFFFFFFFu <<
           (32 - prefixLength);
}

IPv4Address Subnet::getNetworkAddress() const {

    unsigned int network =
        ip.toInteger() &
        getMask();

    return IPv4Address::fromInteger(
        network
    );
}

IPv4Address Subnet::getBroadcastAddress() const {

    unsigned int network =
        getNetworkAddress().toInteger();

    unsigned int broadcast =
        network |
        ~getMask();

    return IPv4Address::fromInteger(
        broadcast
    );
}

IPv4Address Subnet::getFirstHost() const {

    unsigned int network =
        getNetworkAddress().toInteger();

    return IPv4Address::fromInteger(
        network + 1
    );
}

IPv4Address Subnet::getLastHost() const {

    unsigned int broadcast =
        getBroadcastAddress().toInteger();

    return IPv4Address::fromInteger(
        broadcast - 1
    );
}

unsigned int Subnet::getHostCount() const {

    if (prefixLength >= 31)
        return 0;

    return (1u <<
            (32 - prefixLength)) - 2;
}

bool Subnet::contains(
    const IPv4Address& address) const {

    return (
        address.toInteger() &
        getMask()
    ) ==
    getNetworkAddress().toInteger();
}

bool Subnet::sameSubnet(
    const IPv4Address& address) const {

    return contains(address);
}

std::string Subnet::toString() const {

    return getNetworkAddress().toString()
           + "/" +
           std::to_string(prefixLength);
}

void Subnet::display() const {

    std::cout
        << "\n===== SUBNET =====\n";

    std::cout
        << "Network      : "
        << getNetworkAddress().toString()
        << '\n';

    std::cout
        << "Prefix       : /"
        << prefixLength
        << '\n';

    std::cout
        << "Broadcast    : "
        << getBroadcastAddress().toString()
        << '\n';

    std::cout
        << "First Host   : "
        << getFirstHost().toString()
        << '\n';

    std::cout
        << "Last Host    : "
        << getLastHost().toString()
        << '\n';

    std::cout
        << "Host Count   : "
        << getHostCount()
        << '\n';
}