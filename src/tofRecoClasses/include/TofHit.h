#ifndef TofHit_h
#define TofHit_h

// all libraries defined here, better another file?

#include <stdlib.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <TFile.h>
#include <TTree.h>
#include <TMath.h>
#include <TGraph.h>
#include <TF1.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <TROOT.h>
#include <TH2F.h>
#include <TGraphErrors.h>
#include <TStyle.h>
#include <TLine.h>

#include "TofObjectsDict.h"


// Conventions
// - functions and variables are in Pascal case
// - appo variables inside of functions are in snake_case
// - variables and functions of a class always start with the name of the class, without Tof


// Simple function to split strings, might move elsewhere
std::vector<std::string> SplitString(const std::string &, char);

// Adapted gaussian function for fitting
Double_t FitFunction(Double_t *, Double_t *);

class TofHit{
public: 
    // Constructor
    TofHit();
    // TofHit(); // from SAMPIC library
    ~TofHit(){};

    // Functions
    // void HitComputeVariables();
    void HitFitWaveform();
    double HitLinearInterpolation(double);
    double HitComputeCfTime(double);
    void HitQualityCheck();
    void HitMatchDaqChToTofCh();
    char HitGetPlaneId() const;
    void HitDisplayWaveform();
    void HitGetHitInfo() ;
    void HitPrintErrors() ;

    // Setters
    void SetHitFeb(int value) { HitFeb = value; }
    void SetHitFebChannel(int value) { HitFebChannel = value; }
    void SetHitCell0Time(double value) { HitCell0Time = value; }
    void SetHitTotValue(double value) { HitTotValue = value; }
    void SetHitWaveform(std::vector<double> waveform) { HitWaveform = waveform; }
    void SetHitId(int value) { HitId = value; }
    void SetHitSampic(int value) { HitSampic = value; }
    void SetHitDaqChannel(int value) { HitDaqChannel = value; }
    void SetHitPlane(int value) { HitPlane = value; }
    void SetHitBar(int value) { HitBar = value; }
    void SetHitEdge(int value) { HitEdge = value; }
    void SetHitBaseline(double value) { HitBaseline = value; }
    void SetHitRawPeak(double value) { HitRawPeak = value; }
    void SetHitPeak(double value) { HitPeak = value; }
    void SetHitIsSaturated(bool value) { HitIsSaturated = value; }
    void SetHitPeakSample(int value) { HitPeakSample = value; }
    void SetHitPeakTime(double value) { HitPeakTime = value; }
    void SetHitVoltageIntegral(double value) { HitVoltageIntegral = value; }
    void SetHitChannelOnPlane(int value) { HitChannelOnPlane = value; }
    void SetHitRawTotValue(double value) { HitRawTotValue = value; }
    void SetHitUnixTime(double value) { HitUnixTime = value; }
    void SetHitSampleLength(double value) { HitSampleLength = value; }
    void SetHitFitSuccess(bool value) { HitFitSuccess = value; }
    void SetHitFitParameter(double value[6]) { for (int i = 0; i < 6; i++) HitFitParameter[i] = value[i]; }
    void SetHitErrorsList(std::vector<std::string> value) { HitErrorsList = value; }

    // Getters
    const int GetHitFeb() const { return HitFeb; }
    const int GetHitFebChannel()const  { return HitFebChannel; }
    const double GetHitCell0Time() const { return HitCell0Time; }
    const double GetHitTotValue()const  { return HitTotValue; }
    const std::vector<double>& GetHitWaveform() const { return HitWaveform; }
    std::vector<double>& GetHitWaveform() { return HitWaveform; }
    const int GetHitID()const  { return HitId; }
    const int GetHitSampic() const { return HitSampic; }
    const int GetHitDaqChannel() const { return HitDaqChannel; }
    const int GetHitPlane()const  { return HitPlane; }
    const int GetHitBar()const  { return HitBar; }
    const int GetHitEdge() const { return HitEdge; }
    const double GetHitBaseline() const { return HitBaseline; }
    const double GetHitRawPeak() const { return HitRawPeak; }
    const double GetHitPeak() const { return HitPeak; }
    const bool GetHitIsSaturated() const { return HitIsSaturated; }
    const int GetHitPeakSample() const { return HitPeakSample; }
    const double GetHitPeakTime() const { return HitPeakTime; }
    const double GetHitVoltageIntegral() const { return HitVoltageIntegral; }
    const int GetHitChannelOnPlane() const { return HitChannelOnPlane; }
    const double GetHitRawTotValue() const { return HitRawTotValue; }
    const double GetHitUnixTime() const { return HitUnixTime; }
    const double GetHitSampleLength() const { return HitSampleLength; }
    const bool GetHitFitSuccess() const { return HitFitSuccess; }
    const double* GetHitFitParameter() const { return HitFitParameter; }
    const TF1 GetHitFitFunction() const { return HitFitFunction; }

private:

    // Variables
    // Read from input file
    int HitFeb {-1};
    int HitFebChannel{-1};
    double HitCell0Time{-1};
    double HitTotValue{-1};
    std::vector<double> HitWaveform = {};

    // Computed
    int HitId{-1};
    int HitSampic{-1};
    int HitDaqChannel{-1}; // not too strictly needed but better to have it
    int HitPlane{-1}; // dictionary is U=0, D=1, T=2, B=3, L=4, R=5
    int HitBar{-1};
    int HitEdge{-1}; // 0 is L, 1 is R
    double HitBaseline{-1};
    double HitRawPeak{-1};
    double HitPeak{-1};
    int HitPeakSample{-1};
    double HitPeakTime{-1}; // for now it's inside readout window
    double HitVoltageIntegral{-1};
    bool HitIsSaturated{false};

    // Might remove
    int HitChannelOnPlane{-1};
    double HitRawTotValue{-1};
    double HitUnixTime{-1};

    // Fit
    double HitSampleLength{-1}; // taken from run
    bool HitFitSuccess {false};
    double HitFitParameter[6];
    TF1 HitFitFunction {"HitFitFunction", FitFunction, 0, 0, 6};

    std::vector <std::string> HitErrorsList = {}; 
};


#endif
