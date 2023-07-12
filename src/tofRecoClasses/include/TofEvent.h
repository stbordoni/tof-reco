#ifndef TofEvent_h
#define TofEvent_h

#include "TofSignal.h"
#include "TofHit.h"

#include "SAMPIC_256Ch_Type.h"

#include "vector"



class TofEvent{
public:

  // Constructor
  TofEvent();
  ~TofEvent() = default;

  // Setters
  void SetEventSize(int value) { EventSize = value; }
  void AddHit(const TofHit& value) { EventHitsList.emplace_back(value); }

  // Getters
  [[nodiscard]] int GetEventSize() const {return EventSize;}
  [[nodiscard]] double GetEventTimeOfFlight() const {return EventTimeOfFlight;}
  [[nodiscard]] const std::vector <TofSignal>& GetEventSignalsList() const {return EventSignalsList;}
  [[nodiscard]] const std::vector <TofHit>& GetEventHitsList() const {return EventHitsList;}

  // Core
  void EventGetEventInfo();
  void EventCreateSignals();
  void EventComputeTimeOfFlight();

private:

  // Variables
  int EventSize {0};
  double EventTimeOfFlight {0}; // can be more than one element
  std::vector<TofSignal> EventSignalsList;
  std::vector<TofHit> EventHitsList; // remove?

};

#endif
