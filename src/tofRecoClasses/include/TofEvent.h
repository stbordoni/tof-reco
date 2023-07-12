#ifndef TofEvent_h
#define TofEvent_h

#include "TofSignal.h"
#include "TofHit.h"

#include "SAMPIC_256Ch_Type.h"

#include "vector"



class TofEvent{
public:

  // Constructor
  TofEvent() = default;
  ~TofEvent() = default;

  // Setters
  void AddHit(const TofHit& value) { EventHitsList.emplace_back(value); }

  // Getters
  [[nodiscard]] double GetEventTimeOfFlight() const {return EventTimeOfFlight;}
  [[nodiscard]] const std::vector <TofSignal>& GetEventSignalsList() const {return EventSignalsList;}
  [[nodiscard]] const std::vector <TofHit>& GetEventHitsList() const {return EventHitsList;}

  // Core
  void printEventInfo();
  void EventCreateSignals();
  void EventComputeTimeOfFlight();

private:

  // Variables
  double EventTimeOfFlight {0}; // can be more than one element
  std::vector<TofHit> EventHitsList; // remove?
  std::vector<TofSignal> EventSignalsList;

};

#endif
