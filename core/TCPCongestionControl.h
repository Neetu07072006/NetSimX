#ifndef TCPCONGESTIONCONTROL_H
#define TCPCONGESTIONCONTROL_H
#include <string>
class TCPCongestionControl{
private:
    double congestionWindow;
    double slowStartThreshold;
    double maximumWindow;
    int duplicateACKs;
    int timeoutCount;
    int acknowledgedSegments;
    int lostSegments;
    std::string state;
public:
    TCPCongestionControl(double initialWindow=1.0,double slowStartThreshold=16.0,double maximumWindow=64.0);
    double getCongestionWindow() const;
    double getSlowStartThreshold() const;
    double getMaximumWindow() const;
    int getDuplicateACKs() const;
    int getTimeoutCount() const;
    int getAcknowledgedSegments() const;
    int getLostSegments() const;
    std::string getState() const;
    void onACK();
    void onDuplicateACK();
    void onTimeout();
    void onPacketLoss();
    void reset();
    void display() const;
};
#endif