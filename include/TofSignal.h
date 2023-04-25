#ifndef TofSignal_h
#define TofSignal_h

#include "TofHit.h"


class TofSignal{
public:
    // constructor
    TofSignal();
    ~TofSignal(){};

    // variables
    bool SignalBothEdges;
    // vector of hits or this, choose
    TofHit SignalHitLeft; 
    TofHit SignalHitRight; 
    std::vector <TofHit> SignalHitsList;
    
    // functions
    void SignalQualityCheck();
    void SignalIdentifyEdges();
    void SignalGetSignalInfo();

};

#endif
