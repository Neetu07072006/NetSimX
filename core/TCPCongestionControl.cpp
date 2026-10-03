#include "TCPCongestionControl.h"
#include <iostream>
TCPCongestionControl::TCPCongestionControl(double initialWindow,double slowStartThreshold,double maximumWindow):congestionWindow(initialWindow),slowStartThreshold(slowStartThreshold),maximumWindow(maximumWindow),duplicateACKs(0),timeoutCount(0),acknowledgedSegments(0),lostSegments(0),state("SLOW START"){}
double TCPCongestionControl::getCongestionWindow() const{return congestionWindow;}
double TCPCongestionControl::getSlowStartThreshold() const{return slowStartThreshold;}
double TCPCongestionControl::getMaximumWindow() const{return maximumWindow;}
int TCPCongestionControl::getDuplicateACKs() const{return duplicateACKs;}
int TCPCongestionControl::getTimeoutCount() const{return timeoutCount;}
int TCPCongestionControl::getAcknowledgedSegments() const{return acknowledgedSegments;}
int TCPCongestionControl::getLostSegments() const{return lostSegments;}
std::string TCPCongestionControl::getState() const{return state;}
void TCPCongestionControl::onACK(){
    ++acknowledgedSegments;
    duplicateACKs=0;
    if(congestionWindow<slowStartThreshold){
        congestionWindow*=2.0;
        state="SLOW START";
    }else{
        congestionWindow+=1.0;
        state="CONGESTION AVOIDANCE";
    }
    if(congestionWindow>maximumWindow)congestionWindow=maximumWindow;
}
void TCPCongestionControl::onDuplicateACK(){
    ++duplicateACKs;
    if(duplicateACKs>=3){
        slowStartThreshold=congestionWindow/2.0;
        if(slowStartThreshold<1.0)slowStartThreshold=1.0;
        congestionWindow=slowStartThreshold;
        state="FAST RECOVERY";
        ++lostSegments;
        duplicateACKs=0;
    }
}
void TCPCongestionControl::onTimeout(){
    ++timeoutCount;
    ++lostSegments;
    slowStartThreshold=congestionWindow/2.0;
    if(slowStartThreshold<1.0)slowStartThreshold=1.0;
    congestionWindow=1.0;
    state="SLOW START";
    duplicateACKs=0;
}
void TCPCongestionControl::onPacketLoss(){
    ++lostSegments;
    slowStartThreshold=congestionWindow/2.0;
    if(slowStartThreshold<1.0)slowStartThreshold=1.0;
    congestionWindow=slowStartThreshold;
    state="CONGESTION AVOIDANCE";
    duplicateACKs=0;
}
void TCPCongestionControl::reset(){
    congestionWindow=1.0;
    slowStartThreshold=16.0;
    duplicateACKs=0;
    timeoutCount=0;
    acknowledgedSegments=0;
    lostSegments=0;
    state="SLOW START";
}
void TCPCongestionControl::display() const{
    std::cout<<"\n===== TCP CONGESTION CONTROL =====\n";
    std::cout<<"Algorithm             : TCP Reno\n";
    std::cout<<"State                 : "<<state<<'\n';
    std::cout<<"Congestion Window     : "<<congestionWindow<<" segments\n";
    std::cout<<"Slow Start Threshold  : "<<slowStartThreshold<<" segments\n";
    std::cout<<"Maximum Window        : "<<maximumWindow<<" segments\n";
    std::cout<<"Duplicate ACKs        : "<<duplicateACKs<<'\n';
    std::cout<<"Timeouts              : "<<timeoutCount<<'\n';
    std::cout<<"Acknowledged Segments : "<<acknowledgedSegments<<'\n';
    std::cout<<"Lost Segments         : "<<lostSegments<<'\n';
}