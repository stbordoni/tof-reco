#ifndef TofEvent_h
#define TofEvent_h

#include "TofSignal.h"



class TofEvent{
public: 
    
    // variables
    int EventSize; // number of signals in event, add also hits?

    // Constructor
    TofEvent();
    ~TofEvent(){};
    std::vector <TofSignal> EventSignalsList;
    std::vector <TofHit> EventHitsList; // remove?

    // Functions
    void EventCreateSignals();
    void EventGetEventInfo();

};

#endif
