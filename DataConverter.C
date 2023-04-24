//
// Created by E. Villa on Mar 28th, 2023. 
// emanuele.villa@cern.ch
// 

// #include "TofObjects.h"
// #include "TofObjects.C"
#include "TofHit.h"
#include "TofSignal.h"
#include "TofEvent.h"
#include "TofRun.h"


void DataConverter(std::string software, std::string run_full_path, std::string output_directory){
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
        return;
    }

    thisRun.RunQualityCheck();
    thisRun.RunSetAnalysisOptions();
    thisRun.RunGetInfo(); // comment to reduce output
    thisRun.RunLoadHits();
    thisRun.RunOrderHits();
    thisRun.RunPrintErrors();
    thisRun.RunGenerateOutputFile(output_directory);

    // generate outputfile  
    // std::string outputfile = output_directory + thisRun.RunAddress + ".root";
    // TFile *f = new TFile(outputfile.c_str(), "RECREATE");
    // TTree *t = new TTree("T", "TofTree");
    // TofEvent *event = new TofEvent();
    // t->Branch("event", &event);
    // std::cout << "Output file: " << outputfile << std::endl;
    // close outputfile
    

}