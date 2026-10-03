#include "IPv4Address.h"
#include <sstream>
#include <vector>

IPv4Address::IPv4Address()
    : address(0), valid(false) {}

IPv4Address::IPv4Address(
    const std::string& ip)
    : address(0), valid(false) {

    std::stringstream ss(ip);
    std::string part;
    std::vector<int> octets;

    while (std::getline(ss, part, '.')) {

        if (part.empty())
            return;

        int value = std::stoi(part);

        if (value < 0 || value > 255)
            return;

        octets.push_back(value);
    }

    if (octets.size() != 4)
        return;

    address =
        (static_cast<unsigned int>(octets[0]) << 24) |
        (static_cast<unsigned int>(octets[1]) << 16) |
        (static_cast<unsigned int>(octets[2]) << 8) |
        static_cast<unsigned int>(octets[3]);

    valid = true;
}

bool IPv4Address::isValid() const {
    return valid;
}

std::string IPv4Address::toString() const {

    if (!valid)
        return "0.0.0.0";

    return std::to_string(
               (address >> 24) & 255
           ) + "." +
           std::to_string(
               (address >> 16) & 255
           ) + "." +
           std::to_string(
               (address >> 8) & 255
           ) + "." +
           std::to_string(
               address & 255
           );
}

unsigned int IPv4Address::toInteger() const {
    return address;
}

IPv4Address IPv4Address::fromInteger(
    unsigned int value) {

    IPv4Address result;

    result.address = value;
    result.valid = true;

    return result;
}

bool IPv4Address::operator==(
    const IPv4Address& other) const {

    return valid == other.valid &&
           address == other.address;
}

bool IPv4Address::operator!=(
    const IPv4Address& other) const {

    return !(*this == other);
}