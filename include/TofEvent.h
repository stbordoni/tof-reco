#ifndef TofEvent_h
#define TofEvent_h

#include "TofSignal.h"



class TofEvent{
public: 
    
    // Constructor
    TofEvent();
    ~TofEvent(){};
    std::vector <TofSignal> EventSignalsList;
    std::vector <TofHit> EventHitsList; // remove?

    // Functions
    void EventCreateSignals();
    void EventGetEventInfo();

    // Setters
    void SetEventSize(int value) { EventSize = value; }

    // Getters
    int GetEventSize() {return EventSize;}
    

private:

    // Variables
    int EventSize {0};

};

#endif
