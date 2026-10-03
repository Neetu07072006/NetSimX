#ifndef MACTABLE_H
#define MACTABLE_H

#include <map>
#include <string>
#include <chrono>

class MACTable {
private:
    struct Entry {
        int port;
        std::chrono::steady_clock::time_point timestamp;
    };

    std::map<std::string, Entry> table;
    int agingTimeSeconds;

public:
    MACTable(int agingTimeSeconds = 60);

    void learn(const std::string& mac, int port);
    int lookup(const std::string& mac);
    bool contains(const std::string& mac);
    void remove(const std::string& mac);
    void clear();
    void ageEntries();

    void display() const;
};

#endif