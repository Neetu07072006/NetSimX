#ifndef SWITCH_H
#define SWITCH_H

#include "NetworkNode.h"
#include "MACTable.h"
#include <string>
#include <vector>

class EthernetFrame;

enum class ForwardingAction {
    FORWARD,
    FLOOD,
    DROP
};

struct ForwardingDecision {
    ForwardingAction action;
    int outputPort;
};

class Switch : public NetworkNode {
private:
    MACTable macTable;
    std::vector<int> activePorts;

public:
    Switch(int id, const std::string& name);

    void addPort(int port);
    bool hasPort(int port) const;

    void learnMAC(const std::string& mac, int port);
    int getPort(const std::string& mac);
    bool knowsMAC(const std::string& mac);

    ForwardingDecision decideForwarding(
        const EthernetFrame& frame,
        int incomingPort
    );

    void processFrame(
        const EthernetFrame& frame,
        int incomingPort
    );

    void ageMACEntries();
    void clearMACTable();

    MACTable& getMACTable();

    void displayPorts() const;
    void display() const override;
};

#endif