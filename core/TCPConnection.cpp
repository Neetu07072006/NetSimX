#include "TCPConnection.h"
#include <iostream>
TCPConnection::TCPConnection(uint32_t initialSequence,uint16_t windowSize,int maxRetransmissions):state(TCPState::CLOSED),localSequence(initialSequence),remoteSequence(0),windowSize(windowSize),retransmissionCount(0),maxRetransmissions(maxRetransmissions){}
TCPState TCPConnection::getState() const{return state;}
uint32_t TCPConnection::getLocalSequence() const{return localSequence;}
uint32_t TCPConnection::getRemoteSequence() const{return remoteSequence;}
uint16_t TCPConnection::getWindowSize() const{return windowSize;}
int TCPConnection::getRetransmissionCount() const{return retransmissionCount;}
void TCPConnection::incrementRetransmission(){++retransmissionCount;}
void TCPConnection::resetRetransmissions(){retransmissionCount=0;}
void TCPConnection::setState(TCPState state){this->state=state;}
void TCPConnection::setRemoteSequence(uint32_t sequence){remoteSequence=sequence;}
void TCPConnection::advanceLocalSequence(uint32_t amount){localSequence+=amount;}
bool TCPConnection::canTransmit() const{return state==TCPState::ESTABLISHED;}
std::string TCPConnection::getStateName() const{
    switch(state){
        case TCPState::CLOSED:return "CLOSED";
        case TCPState::SYN_SENT:return "SYN-SENT";
        case TCPState::SYN_RECEIVED:return "SYN-RECEIVED";
        case TCPState::ESTABLISHED:return "ESTABLISHED";
        case TCPState::FIN_WAIT_1:return "FIN-WAIT-1";
        case TCPState::FIN_WAIT_2:return "FIN-WAIT-2";
        case TCPState::CLOSE_WAIT:return "CLOSE-WAIT";
        case TCPState::LAST_ACK:return "LAST-ACK";
        case TCPState::TIME_WAIT:return "TIME-WAIT";
    }
    return "UNKNOWN";
}
void TCPConnection::display() const{
    std::cout<<"\n===== TCP CONNECTION =====\n";
    std::cout<<"State                : "<<getStateName()<<'\n';
    std::cout<<"Local Sequence       : "<<localSequence<<'\n';
    std::cout<<"Remote Sequence      : "<<remoteSequence<<'\n';
    std::cout<<"Window Size          : "<<windowSize<<'\n';
    std::cout<<"Retransmissions      : "<<retransmissionCount<<'\n';
}