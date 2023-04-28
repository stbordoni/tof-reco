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
    // VARIABLES
    // Read from input file
    int HitFeb;
    int HitFebChannel;
    double HitCell0Time;
    double HitTOTValue;
    std::vector<double> HitWaveform;

    // Computed and needdd
    int HitId;
    int HitSampic;
    int HitDaqChannel; // not too strictly needed but better to have it
    int HitPlane; // dictionary is U=0, D=1, T=2, B=3, L=4, R=5
    int HitBar;
    int HitEdge; // 0 is L, 1 is R
    double HitBaseline;
    double HitRawPeak;
    double HitPeak;
    int HitPeakSample;
    double HitPeakTime;
    double HitVoltageIntegral;

    // might remove
    int HitChannelOnPlane;
    double HitRawTOTValue;
    double HitUnixTime;


    // fit
    double HitSampleLength; // taken from run
    std::vector <double> HitPeakFraction; // taken from run
    bool HitFitSuccess;

    double HitFitParameter[6];
    std::vector <double> HitCfTimeFromFit;

    std::vector <double> HitPeakFractionSampleLinearInt;
    std::vector <double> HitPeakFractionTimeLinearInt;
    std::vector <double> HitPeakFractionSampleSplineInt;
    std::vector <double> HitPeakFractionTimeSplineInt;

    std::vector <std::string> HitErrorsList; // check if better to have only RunErrors
    
    // Constructor
    TofHit();
    ~TofHit(){};

    // Functions
    // void HitComputeVariables();
    void HitFitWaveform();
    void HitQualityCheck();
    void HitMatchDaqChToTofCh();
    char HitGetPlaneId();
    void HitGetHitInfo();

    // getters
    int GetHitId(){return HitId;};

private: 
    // check if to make some variables private 
};


#endif
