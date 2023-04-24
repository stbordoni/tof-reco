#ifndef TofHit_h
#define TofHit_h

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

#include "TofObjectsDict.h"


// Conventions
// - functions and variables are in Pascal case
// - appo variables inside of functions are in snake_case
// - variables and functions of a class always start with the name of the class, without Tof
// Note that classes are nested in mother daughter relations


// Simple function to split strings, might move elsewhere
std::vector<std::string> SplitString(const std::string &, char);




class TofHit{
public: 
    // Variables
    int HitId;
    int HitFeb;
    int HitSampic;
    int HitFebChannel;
    int HitDaqChannel;
    int HitChannelOnPlane; // might remove
    int HitPlane; // dictionary is U=0, D=1, T=2, B=3, L=4, R=5
    int HitBar;
    int HitEdge; // 0 is L, 1 is R
    double HitUnixTime;
    double HitCell0Time;
    double HitTOTValue;
    double HitRawTOTValue;
    double HitRawPeak;
    std::vector<double> HitWaveform;
    double HitBaseline;
    double HitVoltageIntegral;
    double HitPeak;
    int HitPeakSample;
    double HitPeakTime;

    // fit
    double HitFitParameter[6];
    std::vector<double> HitVoltageValueFromFit;

    std::vector<double> HitPeakFractionSampleLinearInt;
    std::vector<double> HitPeakFractionTimeLinearInt;
    std::vector<double> HitPeakFractionSampleSplineInt;
    std::vector<double> HitPeakFractionTimeSplineInt;

    std::vector <std::string> HitErrorsList; // check if better to have only RunErrors

    
    
    // Constructor
    TofHit();
    ~TofHit(){};

    // Functions
    // void HitComputeVariables();
    double HitFitWaveform();
    void HitComputeCf();
    void HitQualityCheck();
    void HitMatchDaqChToTofCh();
    char HitGetPlaneId();
    void HitGetHitInfo();

private: 
    // check if to make some variables private 
};


#endif
