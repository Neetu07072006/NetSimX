#ifndef ARPCACHE_H
#define ARPCACHE_H

#include "IPv4Address.h"
#include <map>
#include <string>
#include <chrono>

class ARPCache {
private:
    struct Entry {
        std::string mac;
        std::chrono::steady_clock::time_point timestamp;
    };

    std::map<unsigned int, Entry> cache;
    int timeoutSeconds;

public:
    ARPCache(int timeoutSeconds = 60);

    void add(const IPv4Address& ip, const std::string& mac);
    bool contains(const IPv4Address& ip);
    std::string lookup(const IPv4Address& ip);
    void remove(const IPv4Address& ip);
    void clear();
    void ageEntries();

    void display() const;
};

#endif