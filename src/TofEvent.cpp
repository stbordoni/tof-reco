#include "../include/TofHit.h"
#include "../include/TofEvent.h"
#include "../include/TofHit.h"
#include "../include/TofSignal.h"

TofEvent::TofEvent(){

}

void TofEvent::EventCreateSignals(){
    TofSignal new_signal;
    bool new_signal_created = true;
    int this_hit_bar = -1, this_hit_plane = -1;
    std::vector <int> skip_elements;

    for (int ihit = 0; ihit < EventHitsList.size(); ihit++){
        
        new_signal_created = false;

        for (int iskip = 0; iskip < skip_elements.size(); iskip++){
            if (ihit == skip_elements.at(iskip)) continue;
        }

        this_hit_bar = EventHitsList.at(ihit).GetHitBar();
        this_hit_plane = EventHitsList.at(ihit).GetHitPlane();
        // std::cout << "New signal for bar " << this_hit_bar << std::endl;

        for (int ihit2 = ihit+1; ihit2 < EventHitsList.size(); ihit2++){
            
            if (skip_elements.size() > 0){
                for (int iskip = 0; iskip < skip_elements.size(); iskip++){
                    if (ihit2 == skip_elements.at(iskip)) continue;
                }
            }

            if (EventHitsList.at(ihit2).GetHitBar() == this_hit_bar && EventHitsList.at(ihit2).GetHitPlane() == this_hit_plane){

                skip_elements.push_back(ihit2);
                EventSignalsList.push_back(TofSignal(EventHitsList.at(ihit), EventHitsList.at(ihit2)));
                new_signal_created = true;
                break;
            }
        }
        
        if  (new_signal_created == false)
            EventSignalsList.push_back(TofSignal(EventHitsList.at(ihit)));

        new_signal_created = true;
    
    }

    EventSize = EventSignalsList.size();
    // std::cout << "Created " << EventSignalsList.size() << " signals" << std::endl;

}

void TofEvent::EventComputeTimeOfFlight(){

    // doing this only when the event contains exactly 2 signals 
    if (EventSize != 2) return;
    std::cout << "this event has size 2 and signals have time " << EventSignalsList.at(0).GetSignalTime() << " and " << EventSignalsList.at(1).GetSignalTime() << std::endl;
    if ( EventSignalsList.at(1).GetSignalTime() == -1){
        // print signalposition of the signal
        std::cout << "Signal position: " << EventSignalsList.at(1).GetSignalPosition() << std::endl;
        

    }

    if (EventSignalsList.at(0).GetSignalTime() != -1 && EventSignalsList.at(1).GetSignalTime() != -1){

        EventTimeOfFlight = abs(EventSignalsList.at(1).GetSignalTime() - EventSignalsList.at(0).GetSignalTime());
        std::cout << "Computed time of flight: " << EventTimeOfFlight << std::endl;
    }

}


void TofEvent::EventGetEventInfo(){
    std::cout << "Event size: " << EventSize << std::endl;
}

