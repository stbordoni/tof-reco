#include "../include/TofRun.h"
#include "../include/TofEvent.h"
#include "../include/TofHit.h"
#include "../include/TofSignal.h"


TofSignal::TofSignal(TofHit one_hit){
    if (one_hit.GetHitEdge() == 0){
        SignalHitMin = one_hit;
        SignalType = 1;
    }
    else if (one_hit.GetHitEdge() == 1){
        SignalHitMax = one_hit;
        SignalType = 2;
        
    }
    else
        std::cerr << "Error in TofSignal constructor, hit has no edge 0 or 1\n";

    SignalGetSignalInfo();
}

TofSignal::TofSignal(TofHit first_hit, TofHit second_hit){

    if (first_hit.GetHitBar() != second_hit.GetHitBar()){
        std::string this_error = "Error in TofSignal constructor, hits are not in the same bar\n";
        SignalErrorsList.push_back(this_error);
        std::cerr << this_error;
        exit(1);
    }

    if (first_hit.GetHitEdge() == 0 && second_hit.GetHitEdge() == 1){
        SignalHitMin = first_hit;
        SignalHitMax = second_hit;
        SignalType = 3;
    }
    else if (first_hit.GetHitEdge() == 1 && second_hit.GetHitEdge() == 0){ 
        std::string this_error = "Warning: calling TofSignal(hit_max, hit_min) instead of TofSignal(hit_min, hit_max) \n";
        std::cerr << this_error;
        SignalErrorsList.push_back(this_error);

        SignalHitMin = second_hit;
        SignalHitMax = first_hit;
        SignalType = 3;
    }
    else
        std::cerr << "Error in TofSignal constructor, hits have no edges 0 and 1\n";

    SignalGetSignalInfo();
}

void TofSignal::SignalQualityCheck(){

}

void TofSignal::SignalGetSignalInfo(){
    std::cout << "SignalType: " << SignalType << std::endl;
    if (SignalType == 1){
        std::cout << "SignalHitMin: " << std::endl;
        SignalHitMin.HitGetHitInfo();
    }
    else if (SignalType == 2){
        std::cout << "SignalHitMax: " << std::endl;
        SignalHitMax.HitGetHitInfo();
    }
    else if (SignalType == 3){
        std::cout << "SignalHitMin: " << std::endl;
        SignalHitMin.HitGetHitInfo();
        std::cout << "SignalHitMax: " << std::endl;
        SignalHitMax.HitGetHitInfo();
    }
    else
        std::cerr << "Error in SignalGetSignalInfo, SignalType is not 1, 2 or 3\n";

}

void TofSignal::SignalPrintErrorsList(){
    for (int i = 0; i < SignalErrorsList.size(); i++){
        std::cerr << SignalErrorsList.at(i) << std::endl;
    }
}
