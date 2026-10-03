#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H
#include "TCPSegment.h"
#include <string>
enum class TCPState{CLOSED,SYN_SENT,SYN_RECEIVED,ESTABLISHED,FIN_WAIT_1,FIN_WAIT_2,CLOSE_WAIT,LAST_ACK,TIME_WAIT};
class TCPConnection{
private:
    TCPState state;
    uint32_t localSequence;
    uint32_t remoteSequence;
    uint16_t windowSize;
    int retransmissionCount;
    int maxRetransmissions;
public:
    TCPConnection(uint32_t initialSequence=1000,uint16_t windowSize=65535,int maxRetransmissions=3);
    TCPState getState() const;
    uint32_t getLocalSequence() const;
    uint32_t getRemoteSequence() const;
    uint16_t getWindowSize() const;
    int getRetransmissionCount() const;
    void incrementRetransmission();
    void resetRetransmissions();
    void setState(TCPState state);
    void setRemoteSequence(uint32_t sequence);
    void advanceLocalSequence(uint32_t amount);
    bool canTransmit() const;
    std::string getStateName() const;
    void display() const;
};
#endif