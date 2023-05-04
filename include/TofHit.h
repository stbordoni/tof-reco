#ifndef TofHit_h
#define TofHit_h

// all libraries defined here, better another file?

#include <stdlib.h>
#include <vector>
#include <string>
#include <nlohmann/json.hpp>
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
    double HitComputeCfTime(double);
    void HitQualityCheck();
    void HitMatchDaqChToTofCh();
    char HitGetPlaneId();
    void HitGetHitInfo();
    void HitPrintErrors();

    // Setters
    void SetHitFeb(int value) { HitFeb = value; }
    void SetHitFebChannel(int value) { HitFebChannel = value; }
    void SetHitCell0Time(double value) { HitCell0Time = value; }
    void SetHitTOTValue(double value) { HitTOTValue = value; }
    void SetHitWaveform(std::vector<double> value) { HitWaveform = value; }
    void SetHitId(int value) { HitId = value; }
    void SetHitSampic(int value) { HitSampic = value; }
    void SetHitDaqChannel(int value) { HitDaqChannel = value; }
    void SetHitPlane(int value) { HitPlane = value; }
    void SetHitBar(int value) { HitBar = value; }
    void SetHitEdge(int value) { HitEdge = value; }
    void SetHitBaseline(double value) { HitBaseline = value; }
    void SetHitRawPeak(double value) { HitRawPeak = value; }
    void SetHitPeak(double value) { HitPeak = value; }
    void SetHitPeakSample(int value) { HitPeakSample = value; }
    void SetHitPeakTime(double value) { HitPeakTime = value; }
    void SetHitVoltageIntegral(double value) { HitVoltageIntegral = value; }
    void SetHitChannelOnPlane(int value) { HitChannelOnPlane = value; }
    void SetHitRawTOTValue(double value) { HitRawTOTValue = value; }
    void SetHitUnixTime(double value) { HitUnixTime = value; }
    void SetHitSampleLength(double value) { HitSampleLength = value; }
    void SetHitFitSuccess(bool value) { HitFitSuccess = value; }
    void SetHitFitParameter(double value[6]) { for (int i = 0; i < 6; i++) HitFitParameter[i] = value[i]; }
    void SetHitErrorsList(std::vector<std::string> value) { HitErrorsList = value; }

    // Getters
    int GetHitFeb() { return HitFeb; }
    int GetHitFebChannel() { return HitFebChannel; }
    double GetHitCell0Time() { return HitCell0Time; }
    double GetHitTOTValue() { return HitTOTValue; }
    std::vector<double> GetHitWaveform() { return HitWaveform; }
    int GetHitID() { return HitId; }
    int GetHitSampic() { return HitSampic; }
    int GetHitDaqChannel() { return HitDaqChannel; }
    int GetHitPlane() { return HitPlane; }
    int GetHitBar() { return HitBar; }
    int GetHitEdge() { return HitEdge; }
    double GetHitBaseline() { return HitBaseline; }
    double GetHitRawPeak() { return HitRawPeak; }
    double GetHitPeak() { return HitPeak; }
    int GetHitPeakSample() { return HitPeakSample; }
    double GetHitPeakTime() { return HitPeakTime; }
    double GetHitVoltageIntegral() { return HitVoltageIntegral; }
    int GetHitChannelOnPlane() { return HitChannelOnPlane; }
    double GetHitRawTOTValue() { return HitRawTOTValue; }
    double GetHitUnixTime() { return HitUnixTime; }
    double GetHitSampleLength() { return HitSampleLength; }
    bool GetHitFitSuccess() { return HitFitSuccess; }
    double* GetHitFitParameter() { return HitFitParameter; }

private:

    // Variables
    // Read from input file
    int HitFeb {-1};
    int HitFebChannel{-1};
    double HitCell0Time{-1};
    double HitTOTValue{-1};
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

    // Might remove
    int HitChannelOnPlane{-1};
    double HitRawTOTValue{-1};
    double HitUnixTime{-1};

    // Fit
    double HitSampleLength{-1}; // taken from run
    bool HitFitSuccess {false};
    double HitFitParameter[6];
    TF1 HitFitFunction {"HitFitFunction", FitFunction, 0, 0, 6};

    std::vector <std::string> HitErrorsList = {}; 
};


#endif
