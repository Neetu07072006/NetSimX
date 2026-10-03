#include "MACTable.h"
#include <iostream>

MACTable::MACTable(int agingTimeSeconds)
    : agingTimeSeconds(agingTimeSeconds) {}

void MACTable::learn(const std::string& mac, int port) {
    table[mac] = {
        port,
        std::chrono::steady_clock::now()
    };
}

int MACTable::lookup(const std::string& mac) {
    ageEntries();

    auto it = table.find(mac);

    if (it == table.end())
        return -1;

    it->second.timestamp =
        std::chrono::steady_clock::now();

    return it->second.port;
}

bool MACTable::contains(const std::string& mac) {
    ageEntries();
    return table.find(mac) != table.end();
}

void MACTable::remove(const std::string& mac) {
    table.erase(mac);
}

void MACTable::clear() {
    table.clear();
}

void MACTable::ageEntries() {
    auto now = std::chrono::steady_clock::now();

    for (auto it = table.begin(); it != table.end();) {
        auto elapsed =
            std::chrono::duration_cast<std::chrono::seconds>(
                now - it->second.timestamp
            ).count();

        if (elapsed >= agingTimeSeconds)
            it = table.erase(it);
        else
            ++it;
    }
}

void MACTable::display() const {
    std::cout << "\n===== MAC ADDRESS TABLE =====\n";

    if (table.empty()) {
        std::cout << "Table is empty.\n";
        return;
    }

    std::cout << "MAC Address\tPort\n";

    for (const auto& entry : table) {
        std::cout << entry.first
                  << '\t'
                  << entry.second.port
                  << '\n';
    }
}