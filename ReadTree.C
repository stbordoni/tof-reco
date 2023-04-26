#include "include/TofHit.h"
#include "include/TofEvent.h"
#include "include/TofRun.h"
#include "include/TofEvent.h"
#include "include/TofObjectsDict.h"


void ReadTree (TString filename, TString treename, TString outputname){
    // Open file
    TFile *file = TFile::Open(filename);
    if (!file || file->IsZombie()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        exit(-1);
    }
    // Get tree
    TTree *tree = (TTree*)file->Get(treename);
    if (!tree || tree->IsZombie()) {
        std::cerr << "Error opening tree: " << treename << std::endl;
        exit(-1);
    }
    // Create output file
    TFile *outputfile = new TFile(outputname, "RECREATE");
    if (!outputfile || outputfile->IsZombie()) {
        std::cerr << "Error creating output file: " << outputname << std::endl;
        exit(-1);
    }
    // Create output tree
    TTree *outputtree = new TTree("TofTree", "TofTree");
    if (!outputtree || outputtree->IsZombie()) {
        std::cerr << "Error creating output tree: " << outputname << std::endl;
        exit(-1);
    }
    // Create TofRun object
    TofRun *run = new TofRun();
    // Create TofEvent object
    // TofEvent *event = new TofEvent();
    // Create TofSignal object
    // TofSignal *signal = new TofSignal();
    // Create TofHit object
    // TofHit *hit = new TofHit();
    // Create TofObjectsDict object
    // TofObjectsDict *dict = new TofObjectsDict();
    // Set branches
    tree->SetBranchAddress("Run", &run);
    // tree->SetBranchAddress("Event", &event);
    // tree->SetBranchAddress("Signal", &signal);
    // tree->SetBranchAddress("Hit", &hit);
    // tree->SetBranchAddress("Dict", &dict);



    // Set output branches
    outputtree->Branch("Run", &run);
    // outputtree->Branch("Event", &event);
    // outputtree->Branch("Signal", &signal);
    // outputtree->Branch("Hit", &hit);
    // outputtree->Branch("Dict", &dict);
    // Loop over entries
    for (int i = 0; i < tree->GetEntries(); i++) {
        tree->GetEntry(i);
        // outputtree->Fill();
    }

    std::cout << "run address" << run->RunAddress << std::endl;
    std::cout << "run software"  << run->RunSoftware << std::endl;

}