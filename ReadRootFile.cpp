// include needed libraries for the code below
#include "./include/TofHit.h"
#include "./include/TofSignal.h"
#include "./include/TofEvent.h"
#include "./include/TofRun.h"
#include "./include/TofObjectsDict.h"

int main(int argc, char *argv[]){

    std::string software = argv[1];
    std::cout  << "Software " << software << std::endl;
    std::string run_full_path = argv[2];
    std::cout << "Run path " << run_full_path << std::endl;

    // create one canvas split in 9, and the histograms to plot HitBar, HitPlane, SignalPosition, HitPeak
    TCanvas *c_goodSignals = new TCanvas("c_goodSignals", "GoodSignals", 900, 900);
    c_goodSignals->Divide(2,2);
    TH1F *h_signalBar = new TH1F("h1", "HitBar", 20, -0.5, 19.5);
    TH1F *h_signalPlane = new TH1F("h_signalPlane", "HitPlane", 6, -0.5, 5.5);
    TH1F *h_signalPosition = new TH1F("h_signalPosition", "SignalPosition", 50, 0., 220.);
    TH1F *h_hitPeak = new TH1F("h_hitPeak", "HitPeak", 50, -0.1,1.1 );

    // extract run number from run_full_path, could remove
    std::string run_number_string = run_full_path.substr(run_full_path.find_last_of("/")+1);
    run_number_string = run_number_string.substr(3, run_number_string.find_last_of("."));
    std::cout << "Run number " << run_number_string << std::endl;
    int run_number = std::stoi(run_number_string);

    std::string input_file = run_full_path;
    std::cout << "reading file: " << input_file << std::endl;
    TFile *f = new TFile(input_file.c_str(), "READ");
    if (f->IsZombie()) {
        std::cerr << "Error: failed to open file " << input_file << std::endl;
        return 1;
    }

    TTree *t = (TTree*)f->Get(Form("TreeTofRun%i", run_number));
    TofRun *run = new TofRun(); // empty constructor
    int status = t->SetBranchAddress("TofRun", &run);
    if (status != 0) {
        std::cerr << "Error: SetBranchAddress failed with status " << status << std::endl;
        return 1;
    }

    std::cout << "Number of entries: " << t->GetEntries() << std::endl;
    t->GetEntry(0); // read first entry, can iterate if more than one run in a file

    std::cout << "Run number: " << run->GetRunNumber() << std::endl;
    std::cout << "Run address: " << run->GetRunAddress() << std::endl;
    std::cout << "Number of events: " << run->GetRunEventsList().size() << std::endl;
    
    for (int i = 0; i < run->RunEventsList.size(); i++){

        std::cout << " Event " << i << std::endl;
        // print number of signals in this event
        std::cout << "  Number of signals: " << run->RunEventsList.at(i).GetEventSignalsList().size() << std::endl;

        for (int j = 0; j < run->RunEventsList.at(i).GetEventSignalsList().size(); j++){
            
            // consider only signals having two hits
            if (run->RunEventsList.at(i).GetEventSignalsList().at(j).GetSignalType() != 3) continue;
            
            std::cout << "   Signal " << j << " has both hits" << std::endl;
            h_signalPosition->Fill(run->RunEventsList.at(i).GetEventSignalsList().at(j).GetSignalPosition());
            h_signalBar->Fill(run->RunEventsList.at(i).GetEventSignalsList().at(j).GetSignalHitMin().GetHitBar());
            h_signalPlane->Fill(run->RunEventsList.at(i).GetEventSignalsList().at(j).GetSignalHitMin().GetHitPlane());
            h_signalBar->Fill(run->RunEventsList.at(i).GetEventSignalsList().at(j).GetSignalHitMax().GetHitBar());
            h_signalPlane->Fill(run->RunEventsList.at(i).GetEventSignalsList().at(j).GetSignalHitMax().GetHitPlane());

        }
    }

    // plot histograms
    c_goodSignals->cd(1);
    h_signalBar->Draw("HIST");
    c_goodSignals->cd(2);
    h_signalPlane->Draw("HIST");
    c_goodSignals->cd(3);
    h_signalPosition->Draw("HIST");
    c_goodSignals->cd(4);
    h_hitPeak->Draw("HIST");

    return 0;
}