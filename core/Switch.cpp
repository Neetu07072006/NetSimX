#include "Switch.h"
#include "EthernetFrame.h"
#include <iostream>
#include <algorithm>

Switch::Switch(int id, const std::string& name)
    : NetworkNode(id, name, NodeType::SWITCH) {}

void Switch::addPort(int port) {
    if (!hasPort(port))
        activePorts.push_back(port);
}

bool Switch::hasPort(int port) const {
    return std::find(
        activePorts.begin(),
        activePorts.end(),
        port
    ) != activePorts.end();
}

void Switch::learnMAC(
    const std::string& mac,
    int port) {

    addPort(port);
    macTable.learn(mac, port);
}

int Switch::getPort(const std::string& mac) {
    return macTable.lookup(mac);
}

bool Switch::knowsMAC(const std::string& mac) {
    return macTable.contains(mac);
}

ForwardingDecision Switch::decideForwarding(
    const EthernetFrame& frame,
    int incomingPort) {

    learnMAC(
        frame.getSourceMAC(),
        incomingPort
    );

    std::string destination =
        frame.getDestinationMAC();

    if (destination == "FF:FF:FF:FF:FF:FF") {
        return {
            ForwardingAction::FLOOD,
            -1
        };
    }

    int destinationPort =
        getPort(destination);

    if (destinationPort == -1) {
        return {
            ForwardingAction::FLOOD,
            -1
        };
    }

    if (destinationPort == incomingPort) {
        return {
            ForwardingAction::DROP,
            destinationPort
        };
    }

    return {
        ForwardingAction::FORWARD,
        destinationPort
    };
}

void Switch::processFrame(
    const EthernetFrame& frame,
    int incomingPort) {

    std::cout << "\n===== SWITCH PROCESSING =====\n";

    std::cout << "Incoming Port   : "
              << incomingPort << '\n';

    std::cout << "Source MAC      : "
              << frame.getSourceMAC() << '\n';

    std::cout << "Destination MAC : "
              << frame.getDestinationMAC() << '\n';

    ForwardingDecision decision =
        decideForwarding(
            frame,
            incomingPort
        );

    if (decision.action ==
        ForwardingAction::FLOOD) {

        std::cout << "Destination MAC is unknown or broadcast.\n";
        std::cout << "Action          : FLOOD\n";

        std::cout << "Output Ports    : ";

        for (int port : activePorts) {
            if (port != incomingPort)
                std::cout << port << ' ';
        }

        std::cout << '\n';
    }
    else if (decision.action ==
             ForwardingAction::DROP) {

        std::cout << "Destination is on incoming port.\n";
        std::cout << "Action          : DROP\n";
    }
    else {

        std::cout << "Destination found on port "
                  << decision.outputPort
                  << ".\n";

        std::cout << "Action          : FORWARD\n";
        std::cout << "Output Port     : "
                  << decision.outputPort
                  << '\n';
    }
}

void Switch::ageMACEntries() {
    macTable.ageEntries();
}

void Switch::clearMACTable() {
    macTable.clear();
}

MACTable& Switch::getMACTable() {
    return macTable;
}

void Switch::displayPorts() const {
    std::cout << "\n===== SWITCH PORTS =====\n";

    if (activePorts.empty()) {
        std::cout << "No active ports.\n";
        return;
    }

    for (int port : activePorts)
        std::cout << "Port " << port << " ACTIVE\n";
}

void Switch::display() const {
    std::cout << "Switch | ID: "
              << id
              << " | Name: "
              << name
              << " | Links: "
              << links.size()
              << " | Ports: "
              << activePorts.size()
              << '\n';
}