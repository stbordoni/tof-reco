//
// Created by E. Villa on Mar 28th, 2023. 
// emanuele.villa@cern.ch
// 

// #include "TofObjects.h"
// #include "TofObjects.C"
#include "./include/TofHit.h"
#include "./include/TofSignal.h"
#include "./include/TofEvent.h"
#include "./include/TofRun.h"
#include "./include/TofObjectsDict.h"


// int main(std::string software, std::string run_full_path, std::string output_directory){
int main(int argc, char *argv[]){

    std::string software = argv[1];
    std::string run_full_path = argv[2];
    std::string output_directory = argv[3];

    TofRun thisRun(software);
    std::cout << "Run path " << run_full_path << std::endl;
    
    if (software == "linux"){
        thisRun.RunSaveSettings_linux(run_full_path);
    } 
    else if (software == "windows"){
        thisRun.RunSaveSettings_windows(run_full_path);
    }
    else {
        std::cerr << "Invalid value for 'software' variable, it has to be 'linux' or 'windows'\n";
        return 0;
    }

    thisRun.RunQualityCheck();
    thisRun.RunSetAnalysisOptions();
    thisRun.RunGetInfo(); // comment to reduce output
    thisRun.RunLoadHits();
    thisRun.RunOrderHits();
    thisRun.RunCreateEvents();
    thisRun.RunPrintErrors();
    thisRun.RunGenerateOutputFile(output_directory);

    // check output file, meaning open it and print out runnumber  
    // std::string outputfile = output_directory + "run" + std::to_string(thisRun.RunNumber) + ".root";
    // std::cout << "reading output file: " << outputfile << std::endl;
    // TFile *f = new TFile(outputfile.c_str(), "READ");
    // TTree *t = (TTree*)f->Get(Form("TreeTofRun%d", thisRun.RunNumber));
    // TofRun *run = new TofRun(software);
    // t->SetBranchAddress("TofRun", &run);
    // t->GetEntry(0);
    // std::cout << "Run number: " << run->RunNumber << std::endl;
    // std::cout << "Run address: " << run->RunAddress << std::endl;
    // // print all channels inside RunEventslist.EventSignalsList
    // std::cout << "Number of events: " << run->RunEventsList.size() << std::endl;
    // // for (int i = 0; i < run->RunEventsList.size(); i++){
    // for (int i = 0; i < 10; i++){
    //     std::cout << "----this event has " << run->RunEventsList[i].EventHitsList.size() << " hits" << std::endl;
    //     for (int j = 0; j < run->RunEventsList[i].EventHitsList.size(); j++){
    //         // std::cout << "--size of cf array " << run->RunEventsList.at(i).EventHitsList.at(j).HitCfTimeFromFit.size() << std::endl;
    //         for (int k = 0; k < run->RunEventsList.at(i).EventHitsList.at(j).HitCfTimeFromFit.size(); k++){
    //             std::cout << "hit " << j << " has " << run->RunEventsList.at(i).EventHitsList.at(j).HitCfTimeFromFit.at(k) << " cf time " <<std::endl;
    //         }
            
    //     }
    // }


    // f->Close();
    // delete f;
    // delete t;
    // delete run;
    // close outputfile


    // generate outputfile  
    // std::string outputfile = output_directory + thisRun.RunAddress + ".root";
    // TFile *f = new TFile(outputfile.c_str(), "RECREATE");
    // TTree *t = new TTree("T", "TofTree");
    // TofEvent *event = new TofEvent();
    // t->Branch("event", &event);
    // std::cout << "Output file: " << outputfile << std::endl;
    // close outputfile
    
    return 0;
}