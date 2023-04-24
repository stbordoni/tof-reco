#include "TofRun.h"
#include "TofEvent.h"
#include "TofHit.h"
#include "TofSignal.h"

TofEvent::TofEvent(){
    EventSize = 0;
    // EventCreateSignals();
}

void TofEvent::EventCreateSignals(){
    TofSignal new_signal;
    bool create_new_signal = true;
    int this_hit_bar = -1;
    std::vector <TofHit> appo_eventshitslist = EventHitsList;
    // print size of EventHitsLits and appo_eventshitslist
    std::vector <int> skip_elements;

    for (int ihit = 0; ihit < appo_eventshitslist.size(); ihit++){
        
        // std::cout << "ihit: " << ihit << std::endl;


        for (int iskip = 0; iskip < skip_elements.size(); iskip++){
            if (ihit == skip_elements.at(iskip)) continue;
        }

        if (create_new_signal) {
            new_signal = TofSignal(); // reset
            this_hit_bar = appo_eventshitslist.at(ihit).HitBar;
            new_signal.SignalHitsList.push_back(appo_eventshitslist.at(ihit));
            // std::cout << "New signal for bar " << this_hit_bar << std::endl;
        }

        for (int ihit2 = ihit+1; ihit2 < appo_eventshitslist.size(); ihit2++){
            
            //print ihit2
            // std::cout << "ihit2: " << ihit2 << std::endl;

            if (skip_elements.size() > 0){
                for (int iskip = 0; iskip < skip_elements.size(); iskip++){
                    if (ihit2 == skip_elements.at(iskip)) continue;
                }
            }

            if (appo_eventshitslist.at(ihit2).HitBar == this_hit_bar){
                // std::cout << "Found hit for other edge of bar " << this_hit_bar << std::endl;
                new_signal.SignalHitsList.push_back(appo_eventshitslist.at(ihit2));
                new_signal.SignalBothEdges = true;
                // remove this element from the vector, already considered
                // appo_eventshitslist.erase(appo_eventshitslist.begin() + ihit2); // not ideal
                // std::cout << "Size of new_signal.SignalHitsList: " << new_signal.SignalHitsList.size() << std::endl;
                skip_elements.push_back(ihit2);
                create_new_signal = true;
                // break;
            }
        }

        // more operations on SignalHits to add?
        // new_signal.SignalIdentifyEdges(); // to debug
        EventSignalsList.push_back(new_signal);
        // std::cout << "Stored new signal for bar " << this_hit_bar << std::endl;
        EventSize++;
        create_new_signal = true;
    
    }

    // std::cout << "Created " << EventSignalsList.size() << " signals" << std::endl;

}

void TofEvent::EventGetEventInfo(){
    std::cout << "Event size: " << EventSize << std::endl;
    for (int isignal = 0; isignal < EventSignalsList.size(); isignal++){
        std::cout << "Signal " << isignal << std::endl;
        std::cout << "Signal size: " << EventSignalsList.at(isignal).SignalHitsList.size() << std::endl;
        std::cout << "Signal bar: " << EventSignalsList.at(isignal).SignalHitsList.at(0).HitBar << std::endl;
        std::cout << "Signal left edge: " << EventSignalsList.at(isignal).SignalHitLeft.HitEdge << std::endl;
        std::cout << "Signal right edge: " << EventSignalsList.at(isignal).SignalHitRight.HitEdge << std::endl;
    }
}

