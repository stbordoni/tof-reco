#ifndef TofRun_h
#define TofRun_h

#include "TofSignal.h"
#include "TofEvent.h"
#include "../submodules/sampic_256ch_linux/include_lib/SAMPIC_256Ch_Type.h"


// TofRun includes the Run settings and all the elements
class TofRun{
public:

    // Constructors
    TofRun(){}; // empty constructor, just to be able to create objects
    ~TofRun(){};

    // Actual events, that are added only after creating the objects
    std::vector <TofHit> RunUnorderedHitsList;
    std::vector <TofHit> RunOrderedHitsList;
    std::vector <TofEvent> RunEventsList;

    // Functions
    void RunReadFilename();
    void RunSaveSettings();
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
    void RunPrintErrors(); // set different ranks: Info, warning, error, fatal?

    // Setters
    void RunSetInputFilePath(const std::string & run_full_path){RunPath = run_full_path;};
    void RunSetSoftwareType(std::string software){RunSoftware = software;};
    void RunSetRunNumber(int run_number){RunNumber = run_number;};
    void RunSetCoincWindow(double coinc_window){RunCoincWindow = coinc_window;};
    // void RunDecodeMidasBank(struct HitStruct*){}; // from MIDAS banks

    // Getters
    std::string GetRunSoftware() {return RunSoftware;}
    int GetRunNumber() {return RunNumber;}
    int GetRunDate() {return RunDate;}
    int GetRunTime() {return RunTime;}
    bool GetRunSinglebarType() {return RunSinglebarType;}
    int GetRunMaxHitsToLoad() {return RunMaxHitsToLoad;}
    std::string GetRunPath() {return RunPath;}
    std::string GetRunAddress() {return RunAddress;}
    std::vector<std::string> GetRunFebSerialNumber() {return RunFebSerialNumber;}
    std::string GetRunFebFirmwareVersion() {return RunFebFirmwareVersion;}
    std::string GetRunControllerBoardSerialNumber() {return RunControllerBoardSerialNumber;}
    std::string GetRunControllerBoardFirmwareVersion() {return RunControllerBoardFirmwareVersion;}
    double GetRunUnixTime() {return RunUnixTime;}
    int GetRunNFebs() {return RunNFebs;}
    int GetRunNSamplesToRead() {return RunNSamplesToRead;}
    double GetRunSamplingFrequency() {return RunSamplingFrequency;}
    double GetRunSampleLength() {return RunSampleLength;}
    double GetRunSamplingTime() {return RunSamplingFrequency;}
    std::vector <TofEvent> GetRunEventsList() {return RunEventsList;}
    double GetRunCoincWindow() {return RunCoincWindow;}

private:

    // Sampic hardware details
    static const int nFebsMax = 4;               
    static const int nSampicsPerFeb = 4;         
    static const int nChannelsPerSampic = 16;    
    static const int nChannelsPerFeb = nSampicsPerFeb*nChannelsPerSampic;
    static const int nChannels = nChannelsPerFeb*nFebsMax;

    // Run details
    std::string RunSoftware;
    int RunNumber;
    int RunDate; // format YearMonthDay
    int RunTime; // format HoursMinutes
    bool RunSinglebarType {false};
    int RunMaxHitsToLoad {1000000}; // just for safety
    std::string RunPath; // this will be dependent on the local machine
    std::string RunAddress; // for linux its currently a file, for windows a folder
    std::vector<std::string> RunFebSerialNumber; // = {"1.12", "1.13", "1.18", "1.19"}; // when a board is missign what happens?
    std::string RunFebFirmwareVersion {}; // 
    std::string RunControllerBoardSerialNumber {};
    std::string RunControllerBoardFirmwareVersion {};
    double RunUnixTime {-1};
    int RunNFebs {0};
    int RunNSamplesToRead {0};
    double RunSamplingFrequency {-1}; // MHz
    double RunSampleLength {-1}; // ns
    double RunBaseline[nFebsMax][nSampicsPerFeb] {}; // V
    double RunPostTrig[nFebsMax][nSampicsPerFeb] {}; 
    double RunTrigThr[nChannels] {};
    int RunTotalHits {-1};

    // Settings for analysis
    bool RunVerboseMode {false};
    bool RunSelectedAnalysisOptions {false}; // false in constructor, set to true when SetAnalysisOptions is called succesfully
    double RunCoincWindow {0}; // ns
    int RunNSamplesInWaveform {0}; // actual samples used, different for windows and linux
    int RunNSamplesToExclude {0}; // default is 1
    int RunBaselineFirstSample {0}; // default, this is already after removing bad samples
    int RunBaselineNSamples {5}; // default is 5
    bool RunDeleteHitsList {true};

};

#endif