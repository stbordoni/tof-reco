#ifndef TofRun_h
#define TofRun_h

#include "TofSignal.h"
#include "TofEvent.h"


// TofRun includes the Run settings and all the elements
class TofRun: public TofHit{
public:

    // Sampic hardware details
    static const int nFebsMax = 4;               
    static const int nSampicsPerFeb = 4;         
    static const int nChannelsPerSampic = 16;    
    static const int nChannelsPerFeb = nSampicsPerFeb*nChannelsPerSampic;
    static const int nChannels = nChannelsPerFeb*nFebsMax;

    // Run details
    std::string RunSoftware;
    std::string RunPath; // this will be dependent on the local machine
    std::string RunAddress; // for linux its currently a file, for windows a folder
    std::vector<std::string> RunFebSerialNumber; // = {"1.12", "1.13", "1.18", "1.19"}; // when a board is missign what happens?
    std::vector<std::string> RunFebFirmwareVersion; // = {"1.12", "1.13", "1.18", "1.19"}; // when a board is missign what happens?
    std::string RunControllerBoardSerialNumber;
    std::string RunControllerBoardFirmwareVersion;
    int RunNumber;
    int RunDate; // format YearMonthDay
    int RunTime; // format HoursMinutes
    double RunUnixTime;
    int RunNFebs;
    int RunNSamplesToRead;
    double RunSamplingFrequency; // MHz
    double RunSampleLength; // ns
    double RunBaseline[nFebsMax][nSampicsPerFeb]; // V
    double RunPostTrig[nFebsMax][nSampicsPerFeb];
    double RunTrigThr[nChannels];

    // Constructors
    TofRun(std::string);         // Linux format
    ~TofRun(){};

    // Actual events, that are added only after creating the objects
    std::vector <TofHit> RunUnorderedHitsList;
    std::vector <TofHit> RunOrderedHitsList;
    std::vector <TofEvent> RunEventsList;

    // Functions
    void RunSaveSettings_linux(std::string);
    void RunSaveSettings_windows(std::string);
    void RunGetInfo ();
    void RunQualityCheck();
    void RunFillHitInfo(TofHit&);
    void RunLoadHits();
    void RunOrderHits();
    void RunCreateEvents();    // this can also free up space 
    void RunGenerateOutputFile(std::string);

    // Settings for the analysis
    void RunSetAnalysisOptions ();
    void RunGetAnalysisOptions ();
    std::vector <std::string> RunErrorsList; // maybe not nice, learn about better methods
    void RunPrintErrors();

    // 


private:

    // Settings for analysis, we dont want to have them change midway through analysis
    bool RunVerboseMode;
    bool RunSelectedAnalysisOptions; // false in constructor, set to true when SetAnalysisOptions is called succesfully
    std::string RunInterpolationType; // spline or linear, deafult is linear
    std::vector<double> RunHitPeakFraction = {0.05,0.10,0.20,0.30,0.40,0.50,0.60,0.70,0.90};
    int RunNSamplesInWaveform; // actual samples used, different for windows and linux
    int RunNSamplesToExclude; // default is 1
    int RunBaselineFirstSample = 0; // default, this is already after removing bad samples
    int RunBaselineNSamples; // default is 5
    bool RunDeleteUnorderedHitsList;
    

};

#endif