#ifndef NETWORKLINK_H
#define NETWORKLINK_H
#include <string>
class NetworkNode;
class NetworkLink{
private:
    NetworkNode* nodeA;
    NetworkNode* nodeB;
    double bandwidth;
    double latency;
    double packetLoss;
    bool active;
public:
    NetworkLink(NetworkNode* nodeA,NetworkNode* nodeB,double bandwidth,double latency,double packetLoss=0.0);
    NetworkNode* getNodeA() const;
    NetworkNode* getNodeB() const;
    double getBandwidth() const;
    double getLatency() const;
    double getPacketLoss() const;
    bool isActive() const;
    void setBandwidth(double bandwidth);
    void setLatency(double latency);
    void setPacketLoss(double packetLoss);
    void setActive(bool active);
    void fail();
    void recover();
    void display() const;
};
#endif