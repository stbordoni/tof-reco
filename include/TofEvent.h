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
    void AddSignal(TofSignal value) { EventSignalsList.push_back(value); }

    // Getters
    int GetEventSize() {return EventSize;}
    std::vector <TofSignal> GetEventSignalsList() {return EventSignalsList;}
    std::vector <TofHit> GetEventHitsList() {return EventHitsList;}

private:

    // Variables
    int EventSize {0};
    std::vector <TofSignal> EventSignalsList;
    std::vector <TofHit> EventHitsList; // remove?


};

#endif
