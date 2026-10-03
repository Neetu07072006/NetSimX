#ifndef IPV4ADDRESS_H
#define IPV4ADDRESS_H

#include <string>

class IPv4Address {
private:
    unsigned int address;
    bool valid;

public:
    IPv4Address();
    IPv4Address(const std::string& ip);

    bool isValid() const;

    std::string toString() const;
    unsigned int toInteger() const;

    static IPv4Address fromInteger(
        unsigned int value);

    bool operator==(
        const IPv4Address& other) const;

    bool operator!=(
        const IPv4Address& other) const;
};

#endif