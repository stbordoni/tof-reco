#ifndef TofSignal_h
#define TofSignal_h

#include "TofHit.h"


class TofSignal{
public:

    // Constructors
    TofSignal(){}; // empty constructor, just to be able to create objects
    TofSignal(TofHit); // single hit
    TofSignal(TofHit, TofHit); // two hits
    ~TofSignal(){};

    // variables
    // vector of hits or this, choose
       
    // Functions
    void SignalQualityCheck();
    void SignalGetSignalInfo();
    void SignalPrintErrorsList();

    // Setters
    void SetSignalType(int value) { SignalType = value; }
    void SetSignalHitMin(TofHit value) { SignalHitMin = value; }
    void SetSignalHitMax(TofHit value) { SignalHitMax = value; }
    void SetSignalErrorsList(std::vector <std::string> value) { SignalErrorsList = value; }

private:

    // variables
    int SignalType {0}; // 0 = unknown, 1 = left, 2 = right, 3 = both
    TofHit SignalHitMin; 
    TofHit SignalHitMax; 
    std::vector <std::string> SignalErrorsList;

};

#endif
