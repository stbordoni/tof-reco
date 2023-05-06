#ifndef TofEvent_h
#define TofEvent_h

#include "TofSignal.h"



class TofEvent{
public: 
    
    // Constructor
    TofEvent();
    ~TofEvent(){};
   
    // Functions
    void EventCreateSignals();
    void EventGetEventInfo();

    // Setters
    void SetEventSize(int value) { EventSize = value; }
    void AddHit(TofHit value) { EventHitsList.push_back(value); }
    void EventComputeTimeOfFlight();

    // Getters
    int GetEventSize() {return EventSize;}
    std::vector <TofSignal> GetEventSignalsList() {return EventSignalsList;}
    std::vector <TofHit> GetEventHitsList() {return EventHitsList;}
    double GetEventTimeOfFlight() {return EventTimeOfFlight;}

private:

    // Variables
    int EventSize {0};
    std::vector <TofSignal> EventSignalsList;
    std::vector <TofHit> EventHitsList; // remove?
    double EventTimeOfFlight {0}; // can be more than one element


};

#endif
