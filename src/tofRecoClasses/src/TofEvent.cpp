#include "TofHit.h"
#include "TofEvent.h"
#include "TofHit.h"
#include "TofSignal.h"


void TofEvent::printEventInfo(){
  std::cout << "Event size: " << EventSignalsList.size() << std::endl;
}
void TofEvent::EventCreateSignals(){

    bool created_new_signal = true;
    int this_hit_bar = -1, this_hit_plane = -1;
    std::vector <int> skip_elements;
    bool print = false;

    // set print to true if size is 4
    // if (EventHitsList.size() == 4) print = true;

    if (print) std::cout << "\nCreating signals for event with " << EventHitsList.size() << " hits" << std::endl;
    for (int ihit = 0; ihit < EventHitsList.size(); ihit++){
        
        created_new_signal = false;
        if (print)std::cout << "ihit " << ihit << std::endl;

        for (int iskip = 0; iskip < skip_elements.size(); iskip++){
            if (ihit == skip_elements.at(iskip)) continue;
        }

        this_hit_bar = EventHitsList.at(ihit).GetHitBar();
        this_hit_plane = EventHitsList.at(ihit).GetHitPlane();
        // std::cout << "New signal for bar " << this_hit_bar << std::endl;

        for (int ihit2 = ihit+1; ihit2 < EventHitsList.size(); ihit2++){
            
            if (skip_elements.size() > 0){
                for (int iskip = 0; iskip < skip_elements.size(); iskip++){
                    if (ihit2 == skip_elements.at(iskip)){
                        if (print)std::cout << "ihit2 " << ihit2 << " is in skip_elements, skipping" << std::endl;
                        continue;
                    }
                }
            }
            if (print)std::cout << "ihit2 " << ihit2 << std::endl;


            if (EventHitsList.at(ihit2).GetHitBar() == this_hit_bar && EventHitsList.at(ihit2).GetHitPlane() == this_hit_plane){

                skip_elements.push_back(ihit2);
                EventSignalsList.push_back(TofSignal(EventHitsList.at(ihit), EventHitsList.at(ihit2)));
                created_new_signal = true;
                if (print)std::cout << "Created new signal with hits " << ihit << " and " << ihit2 << std::endl;
                break;
            }
        }
        
        if  (created_new_signal == false){
            EventSignalsList.push_back(TofSignal(EventHitsList.at(ihit)));
            if (print)std::cout << "Created new signal with hit " << ihit << std::endl;
        }

        created_new_signal = true;
    
    }

    // std::cout << "Created " << EventSignalsList.size() << " signals" << std::endl;
}
void TofEvent::EventComputeTimeOfFlight(){

    // if there are exactly two signals having SignalType == 3, then compute the time of flight
    int good_signals = 0;
    TofSignal first_signal, second_signal;
    for (auto signalit : EventSignalsList){
        if (signalit.GetSignalType() == 3) {
            if (good_signals == 2) return;
            good_signals++;
            if (good_signals == 1) first_signal = signalit;
            else second_signal = signalit;
        }

    }
    if (good_signals != 2) return;

    // doing this only when the event contains exactly 2 signals 
    // std::cout << "this event has size 2 and signals have time " << EventSignalsList.at(0).GetSignalTime() << " and " << EventSignalsList.at(1).GetSignalTime();
    if ((first_signal.GetSignalTime() == -1 || second_signal.GetSignalTime() == -1) || (first_signal.GetSignalHitMin().GetHitPlane() == second_signal.GetSignalHitMin().GetHitPlane())){
        // print signalposition of the signal
        // std::cout << " No TOF computed." << std::endl;
        EventTimeOfFlight = -1;
    }
    else{
        EventTimeOfFlight = abs(first_signal.GetSignalTime() - second_signal.GetSignalTime());
        // print signal times
        // std::cout << "First signal time: " << first_signal.GetSignalTime();
        // std::cout << ". Second signal time: " << second_signal.GetSignalTime() << std::endl;
        // std::cout << " Computed time of flight: " << EventTimeOfFlight << std::endl;
    }

}

