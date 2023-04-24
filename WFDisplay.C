#include <set>
#include <utility>      
#include <string>
#include "fstream"

using namespace std;

struct Event{
    vector<int> *HitID=0;
    vector<int> *HitChannelGlobal=0;
    vector<double> *HitPeak=0;
    vector<vector<double>> *HitWaveform=0;
};

void WFDisplay(std::string InputFile, std::string RunID, std::string ch){

    int show_samples = 62;
    double Ymin= -0.1, Ymax = 1.2; //range of amplitudes in the display
    bool show_quickly = true; // set to true to display thw wfs without clocking on canvas. Useful to store all the wfs of one channel wuickly
    
    int CH = stoi(ch);   
    cout << "---\n"; 
    std::cout << "Displaying waveforms of Run " << RunID << ", channel " << CH << std::endl;
    cout << "---\n"; 

    TFile *fInputFile = new TFile(InputFile.c_str(), "READ");
    TTree *EventInfoTree = (TTree*)fInputFile->Get("EventInfoTree");

    Event thisEvent;

    EventInfoTree->SetBranchAddress("HitID",&thisEvent.HitID);
    EventInfoTree->SetBranchAddress("HitChannelGlobal",&thisEvent.HitChannelGlobal);
    EventInfoTree->SetBranchAddress("HitPeak",&thisEvent.HitPeak);
    EventInfoTree->SetBranchAddress("HitWaveform",&thisEvent.HitWaveform);

    int nEvents = EventInfoTree->GetEntries() ;
    
    TCanvas *allWfs = new TCanvas("allWfs", Form("Waveforms CH%i",CH), 600, 500);

    cout << "nEvents in Run"+RunID+": " << nEvents << endl << endl;

    std::ofstream print_wfs;
    print_wfs.open("wfs.csv");
    
    int counter = 0;
    bool hasTriggered;
    for(int iev = 0; iev < nEvents; iev++){

        TGraph *wf = new TGraph();
        EventInfoTree->GetEntry(iev);        
        allWfs->Clear();

        hasTriggered=false;
        for (int ihit = 0; ihit < thisEvent.HitChannelGlobal->size(); ihit++){ 
            if (thisEvent.HitChannelGlobal->at(ihit)==CH){ 
                hasTriggered=true; 
                cout << " CH"+ch+" appears in Event " << iev << ". This Event contains " << thisEvent.HitChannelGlobal->size() << " hits. ";
                cout << fixed << showpoint << setprecision(3);
                cout << "Peak amplitude: " << thisEvent.HitPeak->at(ihit) << setw(2) << "V" << endl;
                wf->Clear();
                for(int w = 0; w < show_samples; w++)  
                    wf->SetPoint(wf->GetN(), w, thisEvent.HitWaveform->at(ihit).at(w));
                wf->GetXaxis()->SetTitle("Samples");
                wf->GetYaxis()->SetTitle("Amplitude [V]");
                wf->SetTitle(Form("Waveforms CH%i , Event %i",CH, iev));           
                wf->GetYaxis()->SetRangeUser(Ymin, Ymax);
                wf->Draw("");
                allWfs->Update();
                if (show_quickly == false) allWfs->WaitPrimitive();
                for(int w = 0; w < show_samples; w++)  
                    print_wfs << thisEvent.HitWaveform->at(ihit)[w] << ", ";
                print_wfs << endl;
                counter++;
            }
            else   
                continue;
        }
    }
    cout << "This channel is triggered " << counter << " times in this event." << endl;
    std::cout << "Note that if no waveform appears on the Canvas, it means that CH"+ch+" has not triggered in Run "+RunID << std::endl;
    print_wfs.close();
} 