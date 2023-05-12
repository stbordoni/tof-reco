#include "../include/TofHit.h"
#include "../include/TofEvent.h"
#include "../include/TofHit.h"
#include "../include/TofSignal.h"

TofEvent::TofEvent(){

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

    EventSize = EventSignalsList.size();
    // std::cout << "Created " << EventSignalsList.size() << " signals" << std::endl;

}

void TofEvent::EventComputeTimeOfFlight(){

    // doing this only when the event contains exactly 2 signals 
    if (EventSize != 2) return;
    std::cout << "this event has size 2 and signals have time " << EventSignalsList.at(0).GetSignalTime() << " and " << EventSignalsList.at(1).GetSignalTime() << std::endl;
    if ( EventSignalsList.at(1).GetSignalTime() == -1){
        // print signalposition of the signal
        std::cout << "SignalTime not computed. Signal position: " << EventSignalsList.at(1).GetSignalPosition() << std::endl;
        

    }

    if (EventSignalsList.at(0).GetSignalTime() != -1 && EventSignalsList.at(1).GetSignalTime() != -1){

        EventTimeOfFlight = abs(EventSignalsList.at(1).GetSignalTime() - EventSignalsList.at(0).GetSignalTime());
        std::cout << "Computed time of flight: " << EventTimeOfFlight << std::endl;
    }

}


void TofEvent::EventGetEventInfo(){
    std::cout << "Event size: " << EventSize << std::endl;
}

