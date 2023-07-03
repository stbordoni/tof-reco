//
// Created by E. Villa on Mar 28th, 2023. 
// emanuele.villa@cern.ch
// 

#include "TofHit.h"
#include "TofRun.h"
#include "TofObjectsDict.h"

#include "TApplication.h"
#include "TH1F.h"

#include "CmdLineParser.h"
#include "Logger.h"

#include "string"
#include "iostream"
#include "vector"

#include "nlohmann/json.hpp"

LoggerInit([]{
  Logger::getUserHeader() << "[" << FILENAME << "]";
});



// int main(std::string software, std::string run_full_path, std::string output_directory){
int main(int argc, char *argv[]){

  CmdLineParser clp;

  clp.getDescription() << "> readTofData is the main interface for unfolding TOF data (DAQ format)"<< std::endl;
  clp.getDescription() << "> and performs a primary event reconstruction." << std::endl;

  clp.addDummyOption("Main options");
  clp.addOption("software", {"-s", "--software"}, "Specify weather if the DAQ software was linux or windows.");
  clp.addOption("runFullPath", {"-r", "--run"}, "Run full path.");
  clp.addOption("outputDir", {"-o", "--output"}, "Specify output directory path");
  clp.addDummyOption();

  // usage allways displayed
  LogInfo << clp.getDescription().str() << std::endl;

  LogInfo << "Usage: " << std::endl;
  LogInfo << clp.getConfigSummary() << std::endl << std::endl;

  clp.parseCmdLine(argc, argv);

  LogThrowIf( clp.isNoOptionTriggered(), "No option was provided." );

  LogInfo << "Provided arguments: " << std::endl;
  LogInfo << clp.getValueSummary() << std::endl << std::endl;

  auto run_full_path = clp.getOptionVal<std::string>("runFullPath");
  auto output_directory = clp.getOptionVal<std::string>("outputDir");

  LogInfo << "Run path " << run_full_path << std::endl;

  TofRun thisRun;
  thisRun.RunSetSoftwareType( clp.getOptionVal<std::string>("software") );
  thisRun.RunSetInputFilePath( run_full_path );
  thisRun.RunReadFilename();
  thisRun.RunSaveSettings();

  thisRun.RunQualityCheck();
  thisRun.RunSetAnalysisOptions();
  thisRun.RunGetInfo(); // comment to reduce output
  thisRun.RunLoadHits();
  thisRun.RunOrderHits();
  thisRun.RunCreateEvents();
  thisRun.RunPrintErrors();

  // read AnalysisSettings.json
  std::string RunAnalysisSettingsFile = "../../../AnalysisSettings.json"; 
    std::ifstream RunAnalysisSettingsStream(RunAnalysisSettingsFile.c_str());
    LogInfo << "Reading analysis settings from " << RunAnalysisSettingsFile << std::endl;
    nlohmann::json analysis_settings_file;
    RunAnalysisSettingsStream >> analysis_settings_file;
    bool waveform_display = analysis_settings_file["HitDisplay"];
    bool run_root_app = analysis_settings_file["PlotsDisplay"];
    bool save_root_tree = analysis_settings_file["SaveTree"]; // you need dictionary enabled for this
    

  // bool waveform_display = false;
  
  LogInfo << "Run number " << thisRun.GetRunNumber() << std::endl;

  //////////////////////////////////////////////////////////////
  // ROOT app and objects
  TApplication *app = new TApplication("myapp", &argc, argv);
  TObjArray *hist_list = new TObjArray();

  TH1F *h_signalBar = new TH1F("h_signalBar", Form("SignalBar, run%i",thisRun.GetRunNumber()), 20, -0.5, 19.5);
  h_signalBar->GetXaxis()->SetTitle("Bar");
  h_signalBar->SetMinimum(0);
  hist_list->Add(h_signalBar);

  TH1F *h_signalPlane = new TH1F("h_signalPlane", Form("SignalPlane, run%i", thisRun.GetRunNumber()), 6, -0.5, 5.5);
  h_signalPlane->GetXaxis()->SetTitle("Plane");
  h_signalPlane->SetMinimum(0);
  hist_list->Add(h_signalPlane);

  TH1F *h_signalPosition = new TH1F("h_signalPosition", Form("SignalPosition, run%i", thisRun.GetRunNumber()), 50, -50, 270.);
  h_signalPosition->GetXaxis()->SetTitle("Position [cm]");
  h_signalPosition->SetMinimum(0);
  hist_list->Add(h_signalPosition);

  TH1F *h_hitPeak = new TH1F("h_hitPeak", Form("HitPeak, run%i", thisRun.GetRunNumber()), 50, -0.1,1.1 );
  h_hitPeak->GetXaxis()->SetTitle("Peak [V]");
  h_hitPeak->SetMinimum(0);
  hist_list->Add(h_hitPeak);

  TH1F *h_singleHitPeak = new TH1F("h_singleHitPeak", Form("SingleHitPeak, run%i", thisRun.GetRunNumber()), 50, -0.1,1.1 );
  h_singleHitPeak->GetXaxis()->SetTitle("Peak [V]");
  h_singleHitPeak->SetMinimum(0);
  hist_list->Add(h_singleHitPeak);

  // TH1F to plot channels firing in a run. On x axis channel number, on y axis number of times that channel fired
  TH1F *h_channelsFiring = new TH1F("h_channelsFiring", Form("ChannelsFiring, run%i", thisRun.GetRunNumber()), 256, -0.5, 255.5);
  h_channelsFiring->GetXaxis()->SetTitle("Channel");
  h_channelsFiring->SetMinimum(0);
  hist_list->Add(h_channelsFiring);

  // array of histos to plot the baseline, rising time, peak sample, max amplitude, for each channel
  const int nChannels = TofRunParameters::nChannels; 
  TH1F *h_baseline[nChannels];
  TH1F *h_maxAmp[nChannels];
  TH1F *h_peakSample [nChannels];
  TH1F *h_risingTime [nChannels];
  TH1F *h_integral [nChannels];
  for (int i = 0; i < nChannels; i++){
    h_baseline[i] = new TH1F(Form("h_baseline_%i", i), Form("Baseline, run%i, channel%i", thisRun.GetRunNumber(), i), 50, -0.2, 0.2);
    h_baseline[i]->GetXaxis()->SetTitle("Baseline [V]");
    hist_list->Add(h_baseline[i]);

    h_maxAmp[i] = new TH1F(Form("h_maxAmp_%i", i), Form("MaxAmp, run%i, channel%i", thisRun.GetRunNumber(), i), 50, -0.1, 1.1);
    h_maxAmp[i]->GetXaxis()->SetTitle("Max Amplitude [V]");
    hist_list->Add(h_maxAmp[i]);

    h_peakSample[i] = new TH1F(Form("h_peakSample_%i", i), Form("PeakSample, run%i, channel%i", thisRun.GetRunNumber(), i), 50, 0, 64);
    h_peakSample[i]->GetXaxis()->SetTitle("Peak Sample [V]");
    hist_list->Add(h_peakSample[i]);

    h_risingTime[i] = new TH1F(Form("h_risingTime_%i", i), Form("RisingTime, run%i, channel%i", thisRun.GetRunNumber(), i), 50, 0, 20);
    h_risingTime[i]->GetXaxis()->SetTitle("Rising Time [V]");
    hist_list->Add(h_risingTime[i]);

    h_integral[i] = new TH1F(Form("h_integral_%i", i), Form("Integral, run%i, channel%i", thisRun.GetRunNumber(), i), 50, 0, 500);
    h_integral[i]->GetXaxis()->SetTitle("Integral [V]");
    hist_list->Add(h_integral[i]);

  }


  // TH1F to plot time of flight
  TH1F *h_timeOfFlight = new TH1F("h_timeOfFlight", Form("TimeOfFlight, run%i", thisRun.GetRunNumber()), 40, -0.5, 55.5);
  h_timeOfFlight->GetXaxis()->SetTitle("Time of Flight [ns]");
  h_timeOfFlight->SetMinimum(0);
  hist_list->Add(h_timeOfFlight);

  TH1F *h_saturatedHits = new TH1F("h_saturatedHits", Form("SaturatedHits, run%i", thisRun.GetRunNumber()), 256, -0.5, 255.5);
  h_saturatedHits->GetXaxis()->SetTitle("Channel");
  h_saturatedHits->SetMinimum(0);
  h_saturatedHits->SetFillColor(kRed);
  h_saturatedHits->SetLineColor(kRed);
  h_saturatedHits->SetFillStyle(3004);
  hist_list->Add(h_saturatedHits);

  TH1F * h_saturatedOtherEdge = new TH1F("h_saturatedOtherEdge", Form("SaturatedOtherEdge, run%i", thisRun.GetRunNumber()), 256, -0.5, 255.5);
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

  // create a vector of histograms with 256 elements, each being a vector of 62 elements
  int nSamples = thisRun.GetRunNSamplesInWaveform();
  // print this and some spacers
  LogInfo << "Number of samples in waveform: " << nSamples << std::endl;
  std::vector<std::vector<std::pair<int,double>>> h_waveforms(nChannels, std::vector<std::pair<int,double>>(nSamples, std::make_pair(0,0)));

  // average waveforms per channel
  std::vector <TGraphErrors*> g_aveWf;
  g_aveWf.reserve(256);
  for (int i = 0; i < 256; i++){
    g_aveWf[i] = new TGraphErrors();
    g_aveWf[i]->SetTitle(Form("baseline_Ch%i", i));
    g_aveWf[i]->SetMarkerStyle(22);
    g_aveWf[i]->SetMarkerSize(0.6);
    g_aveWf[i]->SetMarkerColor(2);
    // set y limits between -0.1 and 1.2
    g_aveWf[i]->SetMinimum(-0.02);
    g_aveWf[i]->SetMaximum(0.22);
    // g_aveWf[i]->SetStats(false);
    // g_aveWf[i]->GetXaxis()->SetTickLength(0);
    // g_aveWf[i]->GetXaxis()->SetLabelOffset(999);
    // g_aveWf[i]->GetYaxis()->SetTickLength(0);
  }

  //////////////////////////////////////////////////////////////

  LogInfo << "Run number: " << thisRun.GetRunNumber() << std::endl;
  LogInfo << "Run address: " << thisRun.GetRunAddress() << std::endl;
  LogInfo << "Number of events: " << thisRun.GetRunEventsList().size() << std::endl;

  int event_counter = 0;
  for (auto  eventit : thisRun.GetRunEventsList()) {
    // LogInfo << "enter event loop" << std::endl;
    for (auto  signalit : eventit.GetEventSignalsList()) {
      // if (event_counter % 100 == 0) {
      //   LogInfo << "Reading through Events, currently at " << event_counter / (double)(thisRun.RunEventsList.size()) * 100 << " %\r";
      //   LogInfo << std::flush;
      // }

      if (signalit.GetSignalType() == 3) {
        auto & thisHitMin = signalit.GetSignalHitMin();
        auto & thisHitMax = signalit.GetSignalHitMax();
        auto thisHitMinChannel = thisHitMin.GetHitDaqChannel();
        auto thisHitMaxChannel = thisHitMax.GetHitDaqChannel();
        h_signalPosition->Fill(signalit.GetSignalPosition());
        h_signalBar->Fill(thisHitMin.GetHitBar());
        h_signalPlane->Fill(thisHitMin.GetHitPlane());
        h_hitPeak->Fill(thisHitMin.GetHitPeak());
        h_channelsFiring->Fill(thisHitMinChannel);
        h_channelsFiring->Fill(thisHitMaxChannel);
        if (thisHitMin.GetHitIsSaturated()){
          h_saturatedHits->Fill(thisHitMinChannel);
          if (thisHitMax.GetHitIsSaturated()) h_saturatedOtherEdge->Fill(thisHitMaxChannel);
        }
        if (thisHitMax.GetHitIsSaturated()){
          h_saturatedHits->Fill(thisHitMaxChannel);
          if (thisHitMin.GetHitIsSaturated()) h_saturatedOtherEdge->Fill(thisHitMinChannel);
        }
        h_planes[thisHitMin.GetHitPlane()]->Fill(signalit.GetSignalPosition(), thisHitMin.GetHitBar());
        g_hits[thisHitMin.GetHitPlane()]->SetPoint(g_hits[thisHitMin.GetHitPlane()]->GetN(), signalit.GetSignalPosition(), thisHitMin.GetHitBar());
        if (waveform_display) signalit.GetSignalHitMin().HitDisplayWaveform();
        if (waveform_display)  signalit.GetSignalHitMax().HitDisplayWaveform();

        h_baseline[thisHitMinChannel]->Fill(thisHitMin.GetHitBaseline());
        h_baseline[thisHitMaxChannel]->Fill(thisHitMax.GetHitBaseline());
        h_maxAmp[thisHitMinChannel]->Fill(thisHitMin.GetHitPeak());
        h_maxAmp[thisHitMaxChannel]->Fill(thisHitMax.GetHitPeak());
        h_peakSample[thisHitMinChannel]->Fill(thisHitMin.GetHitPeakSample());
        h_peakSample[thisHitMaxChannel]->Fill(thisHitMax.GetHitPeakSample());
        // h_risingTime[thisHitMinChannel]->Fill(thisHitMin.HitComputeCfTime(0.9)-thisHitMin.HitComputeCfTime(0.1));
        // h_risingTime[thisHitMaxChannel]->Fill(thisHitMax.HitComputeCfTime(0.9)-thisHitMin.HitComputeCfTime(0.1));
        h_integral[thisHitMinChannel]->Fill(thisHitMin.GetHitVoltageIntegral());
        h_integral[thisHitMaxChannel]->Fill(thisHitMax.GetHitVoltageIntegral());
        // average waveform per channel 
        // loop over samples
        for (int i = 0; i < nSamples; i++){
          h_waveforms.at(thisHitMinChannel).at(i).second += (thisHitMin.GetHitWaveform()[i]);
          h_waveforms.at(thisHitMinChannel).at(i).first ++;
          h_waveforms.at(thisHitMaxChannel).at(i).second += (thisHitMax.GetHitWaveform()[i]);
          h_waveforms.at(thisHitMaxChannel).at(i).first ++;
        }
      }
      else if (signalit.GetSignalType() == 1) {
        auto & thisHitMin = signalit.GetSignalHitMin();
        auto thisHitMinChannel = thisHitMin.GetHitDaqChannel();
        h_singleHitPeak->Fill(thisHitMin.GetHitPeak());
        h_channelsFiring->Fill(thisHitMinChannel);
        if (thisHitMin.GetHitIsSaturated()) h_saturatedHits->Fill(thisHitMinChannel);
        if (waveform_display) thisHitMin.HitDisplayWaveform();

        h_baseline[thisHitMinChannel]->Fill(thisHitMin.GetHitBaseline());
        h_maxAmp[thisHitMinChannel]->Fill(thisHitMin.GetHitPeak());
        h_peakSample[thisHitMinChannel]->Fill(thisHitMin.GetHitPeakSample());
        // h_risingTime[thisHitMinChannel]->Fill(thisHitMin.HitComputeCfTime(0.9)-thisHitMin.HitComputeCfTime(0.1));
        h_integral[thisHitMinChannel]->Fill(thisHitMin.GetHitVoltageIntegral());
        for (int i = 0; i < nSamples; i++){
          h_waveforms.at(thisHitMinChannel).at(i).second += (thisHitMin.GetHitWaveform()[i]);
          h_waveforms.at(thisHitMinChannel).at(i).first ++;
          // h_waveforms.at(thisHitMaxChannel).at(i).second += (thisHitMax.GetHitWaveform()[i]);
          // h_waveforms.at(thisHitMaxChannel).at(i).first ++;
        }
      }
      else if (signalit.GetSignalType() == 2) {
        auto & thisHitMax = signalit.GetSignalHitMax();
        auto thisHitMaxChannel = thisHitMax.GetHitDaqChannel();
        h_singleHitPeak->Fill(thisHitMax.GetHitPeak());
        h_channelsFiring->Fill(thisHitMaxChannel);
        if (thisHitMax.GetHitIsSaturated()) h_saturatedHits->Fill(thisHitMaxChannel);
        if (waveform_display) thisHitMax.HitDisplayWaveform();

        h_baseline[thisHitMaxChannel]->Fill(thisHitMax.GetHitBaseline());
        h_maxAmp[thisHitMaxChannel]->Fill(thisHitMax.GetHitPeak());
        h_peakSample[thisHitMaxChannel]->Fill(thisHitMax.GetHitPeakSample());
        // h_risingTime[thisChannel]->Fill(thisHitMax.HitComputeCfTime(0.9)-thisHitMin.HitComputeCfTime(0.1));
        h_integral[thisHitMaxChannel]->Fill(thisHitMax.GetHitVoltageIntegral());
        for (int i = 0; i < nSamples; i++){
          // h_waveforms.at(thisHitMinChannel).at(i).second += (thisHitMin.GetHitWaveform()[i]);
          // h_waveforms.at(thisHitMinChannel).at(i).first ++;
          h_waveforms.at(thisHitMaxChannel).at(i).second += (thisHitMax.GetHitWaveform()[i]);
          h_waveforms.at(thisHitMaxChannel).at(i).first ++;       }
        }
    }

    if (eventit.GetEventTimeOfFlight() != 0) {
      // LogInfo << "Event has time of flight: " << eventit.GetEventTimeOfFlight() << std::endl;
      h_timeOfFlight->Fill(eventit.GetEventTimeOfFlight());
    }

    event_counter++;
  }

  LogInfo << "\nNow plotting histograms" << std::endl;

  // plot histograms for good channels
  TCanvas *c_allSignals = new TCanvas("c_allSignals", "allSignals", 900, 900);
  c_allSignals->Divide(3,2);
  c_allSignals->cd(1);
  h_signalBar->Draw("HIST");
  c_allSignals->cd(2);
  h_signalPlane->Draw("HIST");
  c_allSignals->cd(3);
  h_signalPosition->Draw("HIST");
  c_allSignals->cd(4);
  h_hitPeak->Draw("HIST");
  c_allSignals->cd(5);
  h_channelsFiring->Draw("HIST");
  c_allSignals->cd(6);
  h_timeOfFlight->Draw("HIST");
  c_allSignals->SaveAs(Form("../../../../TofRootFiles/run%i_allSignals.C", thisRun.GetRunNumber()));
  c_allSignals->SaveAs(Form("../../../../TofRootFiles/run%i_allSignals.pdf", thisRun.GetRunNumber()));

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
  c_badSignals->SaveAs(Form("../../../../TofRootFiles/run%i_badSignals.C", thisRun.GetRunNumber()));
  c_badSignals->SaveAs(Form("../../../../TofRootFiles/run%i_badSignals.pdf", thisRun.GetRunNumber()));

  // plot event display, first adjust color palette

  float max_value_hist = 0.;
  for (int i = 0; i < 6; i++) {
    float this_hist_max = h_planes[i]->GetMaximum();
    if (this_hist_max > max_value_hist) {
      max_value_hist = this_hist_max;
    }
  }

  // Create a color palette based on the maximum value
  int numColors = 100; // Adjust the number of colors as needed
  TColor::InitializeColors(); // Initialize the color palette
  gStyle->SetNumberContours(numColors); // Set the number of color levels
  for (int i = 0; i < 6; i++) {
    h_planes[i]->SetMaximum(max_value_hist);
    h_planes[i]->SetMinimum(0.);
    h_planes[i]->SetContour(numColors); // Set the number of contours for color mapping
  }

  TCanvas* c_planes = new TCanvas("c_eventDisplay", Form("EventDisplay, run%i", thisRun.GetRunNumber()), 600, 400);
  c_planes->Divide(3,2);
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
  c_planes->SaveAs(Form("../../../../TofRootFiles/run%i_eventDisplay.C", thisRun.GetRunNumber()));
  c_planes->SaveAs(Form("../../../../TofRootFiles/run%i_eventDisplay.pdf", thisRun.GetRunNumber()));

  // horizontal lines for better visualization
  for (int i = 0; i < 6; i++) {  
    for (int j = 1; j <= 20; j++) {
      TLine* line = new TLine(0, j - 0.5, 220, j - 0.5);
      h_planes[i]->GetListOfFunctions()->Add(line);
    }
  }


  // monitoring plots
  TCanvas *c_monitoring = new TCanvas("c_monitoring", Form("Monitoring, run %i", thisRun.GetRunNumber()), 900, 900);
  c_monitoring->Divide(3,2);
  c_monitoring->cd(1);
  TGraph *g_baseline = new TGraph();
  for (int i = 0; i < nChannels; i++) {
    g_baseline->SetPoint(i, i, h_baseline[i]->GetMean());
  }
  g_baseline->SetTitle(Form("Baseline, run%i", thisRun.GetRunNumber()));
  g_baseline->SetMarkerStyle(2);
  g_baseline->SetMarkerSize(1);
  g_baseline->SetMarkerColor(kRed);
  g_baseline->Draw("AP");
  c_monitoring->cd(2);
  TGraph *g_maxAmp = new TGraph();
  for (int i = 0; i < nChannels; i++) {
    g_maxAmp->SetPoint(i, i, h_maxAmp[i]->GetMean());
  }
  g_maxAmp->SetTitle(Form("Max Amplitude, run%i", thisRun.GetRunNumber()));
  g_maxAmp->SetMarkerStyle(2);
  g_maxAmp->SetMarkerSize(1);
  g_maxAmp->SetMarkerColor(kRed);
  g_maxAmp->Draw("AP");
  c_monitoring->cd(3);
  TGraph *g_peakSample = new TGraph();
  for (int i = 0; i < nChannels; i++) {
    g_peakSample->SetPoint(i, i, h_peakSample[i]->GetMean());
  }
  g_peakSample->SetTitle(Form("Peak Sample, run%i", thisRun.GetRunNumber()));
  g_peakSample->SetMarkerStyle(2);
  g_peakSample->SetMarkerSize(1);
  g_peakSample->SetMarkerColor(kRed);
  g_peakSample->Draw("AP");
  c_monitoring->cd(4);
  TGraph *g_risingTime = new TGraph();
  for (int i = 0; i < nChannels; i++) {
    g_risingTime->SetPoint(i, i, h_risingTime[i]->GetMean());
  }
  g_risingTime->SetTitle(Form("Rising Time, run%i", thisRun.GetRunNumber()));
  g_risingTime->SetMarkerStyle(2);
  g_risingTime->SetMarkerSize(1);
  g_risingTime->SetMarkerColor(kRed);
  g_risingTime->Draw("AP");
  c_monitoring->cd(5);
  TGraph *g_integral = new TGraph();
  for (int i = 0; i < nChannels; i++) {
    g_integral->SetPoint(i, i, h_integral[i]->GetMean());
  }
  g_integral->SetTitle(Form ("Integral, run%i", thisRun.GetRunNumber()));
  g_integral->SetMarkerStyle(2);
  g_integral->SetMarkerSize(1);
  g_integral->SetMarkerColor(kRed);
  g_integral->Draw("AP");
  // 6 is empty for now
  c_monitoring -> SaveAs(Form("../../../../TofRootFiles/run%i_monitoring.C", thisRun.GetRunNumber()));
  c_monitoring -> SaveAs(Form("../../../../TofRootFiles/run%i_monitoring.pdf", thisRun.GetRunNumber()));

  // one canvas 4x4 with the g_aveWf
  // fill g_aveWf with the averages of the h_waveforms
  for (int i = 0; i < nChannels; i++){
    for (int j = 0; j < nSamples; j++){
      g_aveWf[i]->SetPoint(j, j, h_waveforms[i][j].second/double(h_waveforms[i][j].first));
    }
  }
  
  TCanvas *c_aveWf = new TCanvas("c_aveWf", Form("Average Waveforms, run %i", thisRun.GetRunNumber()), 900, 900);
  c_aveWf->Divide(4,4);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      int canvas_number = (i)*4 + j+1;
      c_aveWf->cd(canvas_number);
      g_aveWf[canvas_number]->Draw("AL");
      for (int k = 1; k < 16; k++) g_aveWf[canvas_number + k]->Draw("L");
    }
  }
  c_aveWf->SaveAs(Form("../../../../TofRootFiles/run%i_aveWf.C", thisRun.GetRunNumber()));
  c_aveWf->SaveAs(Form("../../../../TofRootFiles/run%i_aveWf.pdf", thisRun.GetRunNumber()));


  // save all files in a histogram list  and output to a root file
  TFile *f_out = new TFile(Form("../../../../TofRootFiles/run%i_histos.root", thisRun.GetRunNumber()), "RECREATE");
  f_out->cd();
  hist_list->Write();
  f_out->Close();

  if (run_root_app) app->Run();
  else LogInfo << "Info: Enable PlotsDisplay in AnalysisSettings.json to see plots at end of execution.\n";

  if (save_root_tree) thisRun.RunGenerateOutputFile(output_directory);

  return 0;
}