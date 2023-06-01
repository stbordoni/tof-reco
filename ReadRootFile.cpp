// include needed libraries for the code below
#include "./include/TofHit.h"
#include "./include/TofSignal.h"
#include "./include/TofEvent.h"
#include "./include/TofRun.h"
#include "./include/TofObjectsDict.h"
#include <TApplication.h>

int main(int argc, char *argv[]){

    std::string software = argv[1];
    std::cout  << "Software " << software << std::endl;
    std::string run_full_path = argv[2];
    std::cout << "Run path " << run_full_path << std::endl;
    std::string run_number_string = SplitString(run_full_path.substr(run_full_path.find_last_of("/")+1), '.').at(0);
    run_number_string = run_number_string.substr(run_number_string.find_first_of("n")+1);
    int run_number = std::stoi(run_number_string);
    std::cout << "Run number " << run_number << std::endl;

    //////////////////////////////////////////////////////////////
    // ROOT app and objects
    TApplication *app = new TApplication("myapp", &argc, argv);
    TObjArray *hist_list = new TObjArray();

    TH1F *h_signalBar = new TH1F("h_signalBar", Form("SignalBar, run%i",run_number), 20, -0.5, 19.5);
    h_signalBar->GetXaxis()->SetTitle("Bar");
    h_signalBar->SetMinimum(0);
    hist_list->Add(h_signalBar);

    TH1F *h_signalPlane = new TH1F("h_signalPlane", Form("SignalPlane, run%i", run_number), 6, -0.5, 5.5);
    h_signalPlane->GetXaxis()->SetTitle("Plane");
    h_signalPlane->SetMinimum(0);
    hist_list->Add(h_signalPlane);

    TH1F *h_signalPosition = new TH1F("h_signalPosition", Form("SignalPosition, run%i", run_number), 50, -50, 270.);
    h_signalPosition->GetXaxis()->SetTitle("Position [cm]");
    h_signalPosition->SetMinimum(0);
    hist_list->Add(h_signalPosition);

    TH1F *h_hitPeak = new TH1F("h_hitPeak", Form("HitPeak, run%i", run_number), 50, -0.1,1.1 );
    h_hitPeak->GetXaxis()->SetTitle("Peak [V]");
    h_hitPeak->SetMinimum(0);
    hist_list->Add(h_hitPeak);

    TH1F *h_singleHitPeak = new TH1F("h_singleHitPeak", Form("SingleHitPeak, run%i", run_number), 50, -0.1,1.1 );
    h_singleHitPeak->GetXaxis()->SetTitle("Peak [V]");
    h_singleHitPeak->SetMinimum(0);
    hist_list->Add(h_singleHitPeak);

    // TH1F to plot channels firing in a run. On x axis channel number, on y axis number of times that channel fired
    TH1F *h_channelsFiring = new TH1F("h_channelsFiring", Form("ChannelsFiring, run%i", run_number), 256, -0.5, 255.5);
    h_channelsFiring->GetXaxis()->SetTitle("Channel");
    h_channelsFiring->SetMinimum(0);
    hist_list->Add(h_channelsFiring);

    // TH1F to plot time of flight
    TH1F *h_timeOfFlight = new TH1F("h_timeOfFlight", Form("TimeOfFlight, run%i", run_number), 40, -0.5, 55.5);
    h_timeOfFlight->GetXaxis()->SetTitle("Time of Flight [ns]");
    h_timeOfFlight->SetMinimum(0);
    hist_list->Add(h_timeOfFlight);

    TH1F *h_saturatedHits = new TH1F("h_saturatedHits", Form("SaturatedHits, run%i", run_number), 256, -0.5, 255.5);
    h_saturatedHits->GetXaxis()->SetTitle("Channel");
    h_saturatedHits->SetMinimum(0);
    h_saturatedHits->SetFillColor(kRed);
    h_saturatedHits->SetLineColor(kRed);
    h_saturatedHits->SetFillStyle(3004);
    hist_list->Add(h_saturatedHits);

    TH1F * h_saturatedOtherEdge = new TH1F("h_saturatedOtherEdge", Form("SaturatedOtherEdge, run%i", run_number), 256, -0.5, 255.5);
    h_saturatedOtherEdge->GetXaxis()->SetTitle("Channel");
    h_saturatedOtherEdge->SetMinimum(0); 
    h_saturatedOtherEdge->SetLineColor(kGreen);
    h_saturatedOtherEdge->SetFillStyle(3004);
    hist_list->Add(h_saturatedOtherEdge);

    // display the events

    // not elegant, temporary solution
    std::vector <std::string> PlaneLabels = {"U","D","T","B","L","R"}; // use map instead!
    std::map<std::string, int> PlaneNumbers;
    for (int i= 0; i < 6; i++) PlaneNumbers[PlaneLabels[i]] = i; 

    int hitBar, planeNumber;
    char hitPlane;

    // Set up the canvas and the histograms
    TCanvas* c_planes = new TCanvas("c_eventDisplay", Form("EventDisplay, run%i", run_number), 600, 800);
    c_planes->Divide(3,4);
    std::vector <TH2F*> h_planes; // better static?
    h_planes.reserve(6);
    for (int i = 0; i < 6; i++){
        h_planes[i] = new TH2F(Form("plane%s", PlaneLabels[i].c_str()), Form("plane%s", PlaneLabels[i].c_str()), 1, 0, 220, 20, -0.5, 19.5);
        h_planes[i]->SetStats(false);
        // h_planes[i]->GetXaxis()->SetTickLength(0);
        // h_planes[i]->GetXaxis()->SetLabelOffset(999);
        h_planes[i]->GetYaxis()->SetTickLength(0);
        h_planes[i]->GetYaxis()->SetTitle("Bar number");
        h_planes[i]->GetXaxis()->SetTitle("Position [cm]");
        // horizontal lines for better visualization
        for (int j = 1; j <= 20; j++) {
            TLine* line = new TLine(0, j - 0.5, 220, j - 0.5); 
            h_planes[i]->GetListOfFunctions()->Add(line); 
        }
        hist_list->Add(h_planes[i]);
    }
    std::vector <TGraphErrors*> g_hits;
    g_hits.reserve(6);
    for (int i = 0; i < 6; i++){
        g_hits[i] = new TGraphErrors();
        g_hits[i]->SetTitle(Form("hits_plane%s", PlaneLabels[i].c_str()));
        g_hits[i]->SetMarkerStyle(22);
        g_hits[i]->SetMarkerSize(0.6);
        g_hits[i]->SetMarkerColor(2);
        // g_hits[i]->SetStats(false);
        // g_hits[i]->GetXaxis()->SetTickLength(0);
        // g_hits[i]->GetXaxis()->SetLabelOffset(999);
        // g_hits[i]->GetYaxis()->SetTickLength(0);
    }

    //////////////////////////////////////////////////////////////

    // Open file    
    std::string input_file = run_full_path;
    std::cout << "Reading file: " << input_file << std::endl;
    TFile *f = new TFile(input_file.c_str(), "READ");
    if (f->IsZombie()) {
        std::cerr << "Error: failed to open file " << input_file << std::endl;
        return 1;
    }

    // Get tree and set branch address
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

    int event_counter = 0;
    for (auto  eventit : run->GetRunEventsList()) {
        // std::cout << "enter event loop" << std::endl;
        for (auto  signalit : eventit.GetEventSignalsList()) {
            if (event_counter % 100 == 0) {
                std::cout << "Reading through Events, currently at " << event_counter / (double)(run->RunEventsList.size()) * 100 << " %\r";
                std::cout << std::flush;
            }

            if (signalit.GetSignalType() == 3) {
                h_signalPosition->Fill(signalit.GetSignalPosition());
                h_signalBar->Fill(signalit.GetSignalHitMin().GetHitBar());
                h_signalPlane->Fill(signalit.GetSignalHitMin().GetHitPlane());
                h_hitPeak->Fill(signalit.GetSignalHitMin().GetHitPeak());
                h_channelsFiring->Fill(signalit.GetSignalHitMin().GetHitDaqChannel());
                h_channelsFiring->Fill(signalit.GetSignalHitMax().GetHitDaqChannel());
                if (signalit.GetSignalHitMin().GetHitIsSaturated()){
                    h_saturatedHits->Fill(signalit.GetSignalHitMin().GetHitDaqChannel());
                    if (signalit.GetSignalHitMax().GetHitIsSaturated()) h_saturatedOtherEdge->Fill(signalit.GetSignalHitMax().GetHitDaqChannel());
                } 
                if (signalit.GetSignalHitMax().GetHitIsSaturated()){ 
                    h_saturatedHits->Fill(signalit.GetSignalHitMax().GetHitDaqChannel());
                    if (signalit.GetSignalHitMin().GetHitIsSaturated()) h_saturatedOtherEdge->Fill(signalit.GetSignalHitMin().GetHitDaqChannel());
                }
                h_planes[signalit.GetSignalHitMin().GetHitPlane()]->Fill(signalit.GetSignalPosition(), signalit.GetSignalHitMin().GetHitBar());
                g_hits[signalit.GetSignalHitMin().GetHitPlane()]->SetPoint(g_hits[signalit.GetSignalHitMin().GetHitPlane()]->GetN(), signalit.GetSignalPosition(), signalit.GetSignalHitMin().GetHitBar());
            }
            else if (signalit.GetSignalType() == 1) {
                h_singleHitPeak->Fill(signalit.GetSignalHitMin().GetHitPeak());
                h_channelsFiring->Fill(signalit.GetSignalHitMin().GetHitDaqChannel());
                if (signalit.GetSignalHitMin().GetHitIsSaturated()) h_saturatedHits->Fill(signalit.GetSignalHitMin().GetHitDaqChannel());
            }
            else if (signalit.GetSignalType() == 2) {
                h_singleHitPeak->Fill(signalit.GetSignalHitMax().GetHitPeak());
                h_channelsFiring->Fill(signalit.GetSignalHitMax().GetHitDaqChannel());
                if (signalit.GetSignalHitMax().GetHitIsSaturated()) h_saturatedHits->Fill(signalit.GetSignalHitMax().GetHitDaqChannel());
            }
        }

        if (eventit.GetEventTimeOfFlight() != 0) {
            // std::cout << "Event has time of flight: " << eventit.GetEventTimeOfFlight() << std::endl;
            h_timeOfFlight->Fill(eventit.GetEventTimeOfFlight());
        }

        event_counter++;
    }

    std::cout << "\nNow plotting histograms" << std::endl;

    // plot histograms for good channels
    TCanvas *c_goodSignals = new TCanvas("c_goodSignals", "GoodSignals", 900, 900);
    c_goodSignals->Divide(3,2);
    c_goodSignals->cd(1);
    h_signalBar->Draw("HIST");
    c_goodSignals->cd(2);
    h_signalPlane->Draw("HIST");
    c_goodSignals->cd(3);
    h_signalPosition->Draw("HIST");
    c_goodSignals->cd(4);
    h_hitPeak->Draw("HIST");
    c_goodSignals->cd(5);
    h_channelsFiring->Draw("HIST");
    c_goodSignals->cd(6);
    h_timeOfFlight->Draw("HIST");

    // plot bad signals in some way
    TCanvas *c_badSignals = new TCanvas("c_badSignals", "'Bad' Signals", 900, 900);
    c_badSignals->Divide(2,2);
    c_badSignals->cd(1);
    h_singleHitPeak->Draw("HIST");
    c_badSignals->cd(2);
    h_channelsFiring->Draw("HIST");
    h_saturatedHits->Draw("SAMES");
    c_badSignals->cd(3);
    h_saturatedHits->Draw("HIST");
    h_saturatedOtherEdge->Draw("SAMES");

    // plot event display
    c_planes->cd(1);
    h_planes[PlaneNumbers["T"]]->Draw("COLZ");
    if (g_hits[PlaneNumbers["T"]]->GetN() > 0) g_hits[PlaneNumbers["T"]]->Draw("Psame");
    c_planes->cd(6);
    h_planes[PlaneNumbers["R"]]->Draw("COLZ");
    if (g_hits[PlaneNumbers["R"]]->GetN() > 0) g_hits[PlaneNumbers["R"]]->Draw("Psame");
    c_planes->cd(2);
    h_planes[PlaneNumbers["U"]]->Draw("COLZ");
    if (g_hits[PlaneNumbers["U"]]->GetN() > 0) g_hits[PlaneNumbers["U"]]->Draw("Psame");
    c_planes->cd(3);
    h_planes[PlaneNumbers["L"]]->Draw("COLZ");
    if (g_hits[PlaneNumbers["L"]]->GetN() > 0) g_hits[PlaneNumbers["L"]]->Draw("Psame");
    c_planes->cd(4);
    h_planes[PlaneNumbers["B"]]->Draw("COLZ");
    if (g_hits[PlaneNumbers["B"]]->GetN() > 0) g_hits[PlaneNumbers["B"]]->Draw("Psame");
    c_planes->cd(5);
    h_planes[PlaneNumbers["D"]]->Draw("COLZ");
    if (g_hits[PlaneNumbers["D"]]->GetN() > 0) g_hits[PlaneNumbers["D"]]->Draw("Psame");


    // save all files in a histogram list  and output to a root file
    TFile *f_out = new TFile(Form("../../TofRootFiles/run%i_plots.root", run_number), "RECREATE");
    f_out->cd();
    hist_list->Write();
    f_out->Close();

    app->Run();

    return 0;
}