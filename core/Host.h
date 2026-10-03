#ifndef HOST_H
#define HOST_H

#include "NetworkNode.h"
#include "ARPCache.h"

class Host : public NetworkNode {
private:
    std::string macAddress;
    ARPCache arpCache;

public:
    Host(int id, const std::string& name,
         const std::string& macAddress);

    void setMACAddress(const std::string& mac);
    std::string getMACAddress() const;

    ARPCache& getARPCache();

    void display() const override;
};

#endif