#include "ARPCache.h"
#include <iostream>

ARPCache::ARPCache(int timeoutSeconds)
    : timeoutSeconds(timeoutSeconds) {}

void ARPCache::add(const IPv4Address& ip,
                   const std::string& mac) {
    cache[ip.toInteger()] = {
        mac,
        std::chrono::steady_clock::now()
    };
}

bool ARPCache::contains(const IPv4Address& ip) {
    ageEntries();
    return cache.find(ip.toInteger()) != cache.end();
}

std::string ARPCache::lookup(const IPv4Address& ip) {
    ageEntries();

    auto it = cache.find(ip.toInteger());

    if (it == cache.end())
        return "";

    return it->second.mac;
}

void ARPCache::remove(const IPv4Address& ip) {
    cache.erase(ip.toInteger());
}

void ARPCache::clear() {
    cache.clear();
}

void ARPCache::ageEntries() {
    auto now = std::chrono::steady_clock::now();

    for (auto it = cache.begin(); it != cache.end();) {
        auto elapsed =
            std::chrono::duration_cast<std::chrono::seconds>(
                now - it->second.timestamp
            ).count();

        if (elapsed >= timeoutSeconds)
            it = cache.erase(it);
        else
            ++it;
    }
}

void ARPCache::display() const {
    std::cout << "\n===== ARP CACHE =====\n";

    if (cache.empty()) {
        std::cout << "ARP cache is empty.\n";
        return;
    }

    std::cout << "IP Address\tMAC Address\n";

    for (const auto& entry : cache) {
        IPv4Address ip =
            IPv4Address::fromInteger(entry.first);

        std::cout << ip.toString()
                  << '\t'
                  << entry.second.mac
                  << '\n';
    }
}