#include <iostream>
#include <set>
#include <string>
#include <cstdlib>  
#include <random>
#include <map>
#include <vector>
#include <utility>
#include "TH2F.h"
#include "TCanvas.h"
#include "TGraph.h"


struct Event{
  vector<char> *HitPlane = 0;
  vector<int> *HitBar = 0;
  vector<int> *HitChannelOnPlane=0;
  vector<vector<double>> *HitPeakFractionTimeLinearInt=0;
  vector<vector<double>> *HitPeakFractionTimeSplineInt=0;
};

using namespace std;

void EventDisplay(std::string InputFile, std::string RunID, std::string eventNumber) {

  cout << "---\n"; 
  std::cout << "Displaying Run " << RunID << ", event " << eventNumber << std::endl;
  cout << "---\n"; 

  // Open the input file and get the TTree, set addresses
  TFile *fInputFile = new TFile(InputFile.c_str(), "READ");
  TTree* EventInfoTree = (TTree*)fInputFile->Get("EventInfoTree");

  // not elegant, but easier to handle
  vector <std::string> PlaneLabels = {"T","B","U","D","L","R"};
  std::map<std::string, int> PlaneNumbers;
  for (int i= 0; i < 6; i++) PlaneNumbers[PlaneLabels[i]] = i; 

  double light_v = 16., light_offset = 0.6; // from global velocity fit
  double samp_rate = 312.5; // ps


  Event thisEvent;
  EventInfoTree->SetBranchAddress("HitPlane",&thisEvent.HitPlane);
  EventInfoTree->SetBranchAddress("HitBar",&thisEvent.HitBar);
  EventInfoTree->SetBranchAddress("HitChannelOnPlane",&thisEvent.HitChannelOnPlane);
  EventInfoTree->SetBranchAddress("HitPeakFractionTimeLinearInt",&thisEvent.HitPeakFractionTimeLinearInt);
  EventInfoTree->SetBranchAddress("HitPeakFractionTimeSplineInt",&thisEvent.HitPeakFractionTimeSplineInt);


  // Loop over the events in the TTree
  int nEntries = EventInfoTree->GetEntries();
  cout << "Entries are " << nEntries << endl;
  int hitBar, planeNumber;
  char hitPlane;

  // Set up the canvas and the histograms
  TCanvas* c_planes = new TCanvas("c_ED", "c_ED", 600, 800);
  c_planes->Divide(3,4);
  vector <TH2F*> h_planes;
  h_planes.reserve(6);
  for (int i = 0; i < 6; i++){
    h_planes[i] = new TH2F(Form("plane%s", PlaneLabels[i].c_str()), Form("plane%s", PlaneLabels[i].c_str()), 2, 0, 220, 20, -0.5, 19.5);
    h_planes[i]->SetStats(false);
    h_planes[i]->GetXaxis()->SetTickLength(0);
    h_planes[i]->GetXaxis()->SetLabelOffset(999);
    h_planes[i]->GetYaxis()->SetTickLength(0);
  }
  vector <TGraphErrors*> g_hits;
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
  
  TCanvas* c_recoPos = new TCanvas("c_recoPos", "c_recoPos", 600, 400);
  TH1F * h_recoPos = new TH1F("h_recoPos", "h_recoPos", 20, -30, 250.);


  bool foundEvent = false;
  for (int iev = 0; iev < nEntries; iev++) {
    // if (foundEvent == true) continue;
    EventInfoTree->GetEntry(iev);
    // if (iev != stoi(eventNumber)) continue;
    foundEvent = true;
    cout << "Found Event number " << eventNumber 
      << ", containing " << thisEvent.HitPlane->size() << " hits\n";

    for (Int_t iHit=0; iHit<thisEvent.HitPlane->size(); iHit++) {
      hitBar = thisEvent.HitBar->at(iHit);
      hitPlane = thisEvent.HitPlane->at(iHit);
      string thisPlane(1,thisEvent.HitPlane->at(iHit));
      planeNumber = PlaneNumbers[thisPlane];
      cout << "Hit number " << iHit << ". HitPlane is " << hitPlane << " and HitBar is " << hitBar << endl;
      if (hitBar < 0 || hitBar > 19) continue; // should never happen, but still
      // int barIndex = hitBar / 2;
      // int halfIndex = hitBar % 2;
      // if (halfIndex == 0) h_planes[planeNumber]->SetBinContent(barIndex+1, 1, 1); 
      // else h_planes[planeNumber]->SetBinContent(barIndex+1, 2, 1);
      // cout << "Fill plane " << planeNumber << ", hitbar " << hitBar << endl;

      double time_difference;
      if (iHit+1 < thisEvent.HitBar->size())
        if (thisEvent.HitBar->at(iHit)==thisEvent.HitBar->at(iHit+1)){
          h_planes[planeNumber]->Fill(0.5, hitBar);
          h_planes[planeNumber]->Fill(1.5, hitBar);
          time_difference = thisEvent.HitPeakFractionTimeLinearInt->at(iHit)[0] - thisEvent.HitPeakFractionTimeLinearInt->at(iHit+1)[0];
          cout << "Time L is " << thisEvent.HitPeakFractionTimeLinearInt->at(iHit)[0] << ". Time R is " << thisEvent.HitPeakFractionTimeLinearInt->at(iHit+1)[0] << endl;
          cout << "time diff is " << time_difference << endl;
          double reconstructed_position = ((light_v*(time_difference + light_offset + 4.5))+ 220.)/(2.);
          cout << "reco pos is " << reconstructed_position << endl;
          g_hits[planeNumber]->SetPoint(g_hits[planeNumber]->GetN(), reconstructed_position, hitBar);
          h_recoPos->Fill(reconstructed_position);
        }
    }
  }

  c_recoPos->cd();
  h_recoPos->Draw("");


  c_planes->cd(2);
  h_planes[PlaneNumbers["T"]]->Draw("COLZ");
  if (g_hits[PlaneNumbers["T"]]->GetN() > 0) g_hits[PlaneNumbers["T"]]->Draw("Psame");
  c_planes->cd(4);
  h_planes[PlaneNumbers["R"]]->Draw("COLZ");
  if (g_hits[PlaneNumbers["R"]]->GetN() > 0) g_hits[PlaneNumbers["R"]]->Draw("Psame");
  c_planes->cd(5);
  h_planes[PlaneNumbers["U"]]->Draw("COLZ");
  if (g_hits[PlaneNumbers["U"]]->GetN() > 0) g_hits[PlaneNumbers["U"]]->Draw("Psame");
  c_planes->cd(6);
  h_planes[PlaneNumbers["L"]]->Draw("COLZ");
  if (g_hits[PlaneNumbers["L"]]->GetN() > 0) g_hits[PlaneNumbers["L"]]->Draw("Psame");
  c_planes->cd(8);
  h_planes[PlaneNumbers["B"]]->Draw("COLZ");
  if (g_hits[PlaneNumbers["B"]]->GetN() > 0) g_hits[PlaneNumbers["B"]]->Draw("Psame");
  c_planes->cd(11);
  h_planes[PlaneNumbers["D"]]->Draw("COLZ");
  if (g_hits[PlaneNumbers["D"]]->GetN() > 0) g_hits[PlaneNumbers["D"]]->Draw("Psame");
}