#include "../include/TofRun.h"
#include "../include/TofEvent.h"
#include "../include/TofHit.h"
#include "../include/TofSignal.h"


TofSignal::TofSignal(){
    SignalBothEdges = false;
}

void TofSignal::SignalIdentifyEdges(){

    // add something to identify singlebar

    if (SignalBothEdges == true){
        // std::cout << "Two edges in this Signal" << std::endl;

        if (SignalHitsList.at(0).GetHitEdge() == 0 && SignalHitsList.at(1).GetHitEdge() == 1){
            SignalHitLeft = SignalHitsList.at(0);
            SignalHitRight = SignalHitsList.at(1);
        }
        else if (SignalHitsList.at(0).GetHitEdge() == 1 && SignalHitsList.at(1).GetHitEdge() == 0){
            SignalHitLeft = SignalHitsList.at(1);
            SignalHitRight = SignalHitsList.at(0);
        }
        else{
            // add to errorslist
            // std::cerr << "Error in SignalIdentifyEdges, signal hits list has two elements but they are not left and right\n";
        }
    }
    else if (SignalBothEdges == false){
        // print "Only one edge in this Signal"
        // std::cout << "Only one edge in this Signal" << std::endl;
        if (SignalHitsList.at(0).GetHitEdge() == 0){
            SignalHitLeft = SignalHitsList.at(0);
        }
        else if (SignalHitsList.at(0).GetHitEdge() == 1){
            SignalHitRight = SignalHitsList.at(0);
        }
        else
            std::cerr << "Error in SignalIdentifyEdges, signal hits list has one element but it is not left or right\n";
    }
    else
        std::cerr << "Error in SignalIdentifyEdges, signal hits list has more than two elements\n";
    
    // SignalGetSignalInfo();
}

void TofSignal::SignalQualityCheck(){
    if (SignalHitsList.size() > 2){
        std::cerr << "Error in SignalQualityCheck, signal has more than two hits\n";
    }
    else if (SignalHitsList.size() == 0){
        std::cerr << "Error in SignalQualityCheck, signal has no hits\n";
    }
    else if (SignalHitsList.size() == 1 && SignalBothEdges == false){
        std::cerr << "Error in SignalQualityCheck, signal has one hit but no edges\n";
    }
    else if (SignalHitsList.size() == 2 && SignalBothEdges == true){
        std::cerr << "Error in SignalQualityCheck, signal has two hits but no edges\n";
    }
    
}

void TofSignal::SignalGetSignalInfo(){
    std::cout << "Signal size: " << SignalHitsList.size() << std::endl;
    std::cout << "Signal bar: " << SignalHitsList.at(0).GetHitBar() << std::endl;
    std::cout << "Signal left edge: " << SignalHitLeft.GetHitEdge() << std::endl;
    std::cout << "Signal right edge: " << SignalHitRight.GetHitEdge() << std::endl;
}
