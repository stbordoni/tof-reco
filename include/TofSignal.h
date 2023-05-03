#ifndef TofSignal_h
#define TofSignal_h

#include "TofHit.h"


class TofSignal{
public:

    // Constructors
    TofSignal();
    ~TofSignal(){};

    // variables
    // vector of hits or this, choose
       
    // functions
    void SignalQualityCheck();
    void SignalIdentifyEdges();
    void SignalGetSignalInfo();

private:

    // variables
    int SignalSize {0};
    int SignalType {0}; // 0 = unknown, 1 = left, 2 = right, 3 = both
    TofHit SignalHitLeft; 
    TofHit SignalHitRight; 
    std::vector <TofHit> SignalHitsList;

};

#endif
