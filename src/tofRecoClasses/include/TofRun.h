#ifndef TofRun_h
#define TofRun_h

#include <utility>

#include "TofSignal.h"
#include "TofEvent.h"

#include "SAMPIC_256Ch_Type.h"

#include "string"
#include "vector"

namespace TofRunParameters{
  const int nFebsMax{4};
  const int nSampicsPerFeb{4};
  const int nChannelsPerSampic{16};
  const int nChannelsPerFeb{nSampicsPerFeb * nChannelsPerSampic};
  const int nChannels{nChannelsPerFeb * nFebsMax};
}

// TofRun includes the Run settings and all the elements
class TofRun{
public:

  // Constructors
  TofRun() = default;
  virtual ~TofRun() = default;

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
  void setVerbose(bool verbose_){ _verbose_ = verbose_; };
  void RunSetInputFilePath(const std::string & run_full_path){RunPath = run_full_path;};
  void RunSetSoftwareType(std::string software){RunSoftware = std::move(software);};
  void RunSetRunNumber(int run_number){RunNumber = run_number;};
  void RunSetCoincWindow(double coinc_window){RunCoincWindow = coinc_window;};
  // void RunDecodeMidasBank(struct HitStruct*){}; // from MIDAS banks

  // const-Getters
  [[nodiscard]] bool GetRunSinglebarType() const {return RunSinglebarType;}
  [[nodiscard]] int GetRunNFebs() const {return RunNFebs;}
  [[nodiscard]] int GetRunDate() const {return RunDate;}
  [[nodiscard]] int GetRunTime() const {return RunTime;}
  [[nodiscard]] int GetRunNumber() const {return RunNumber;}
  [[nodiscard]] int GetRunMaxHitsToLoad() const {return RunMaxHitsToLoad;}
  [[nodiscard]] int GetRunNSamplesToRead() const {return RunNSamplesToRead;}
  [[nodiscard]] int GetRunNSamplesInWaveform() const {return RunNSamplesInWaveform;}
  [[nodiscard]] double GetRunUnixTime() const {return RunUnixTime;}
  [[nodiscard]] double GetRunCoincWindow() const {return RunCoincWindow;}
  [[nodiscard]] double GetRunSampleLength() const {return RunSampleLength;}
  [[nodiscard]] double GetRunSamplingTime() const {return RunSamplingFrequency;}
  [[nodiscard]] double GetRunSamplingFrequency() const {return RunSamplingFrequency;}
  [[nodiscard]] const std::string& GetRunSoftware() const { return RunSoftware; }
  [[nodiscard]] const std::string& GetRunPath() const { return RunPath; }
  [[nodiscard]] const std::string& GetRunAddress() const { return RunAddress; }
  [[nodiscard]] const std::string& GetRunFebFirmwareVersion() const {return RunFebFirmwareVersion;}
  [[nodiscard]] const std::string& GetRunControllerBoardSerialNumber() const {return RunControllerBoardSerialNumber;}
  [[nodiscard]] const std::string& GetRunControllerBoardFirmwareVersion() const {return RunControllerBoardFirmwareVersion;}
  [[nodiscard]] const std::vector<std::string>& GetRunFebSerialNumber() const { return RunFebSerialNumber; }
  [[nodiscard]] const std::vector<TofEvent>& GetRunEventsList() const {return RunEventsList;}

  // Actual events, that are added only after creating the objects
  std::vector <TofHit> RunUnorderedHitsList;
  std::vector <TofHit> RunOrderedHitsList;
  std::vector <TofEvent> RunEventsList;

private:

  // user info
  bool _verbose_{false};


  // Run details
  std::string RunSoftware;
  int RunNumber{};
  int RunDate{}; // format YearMonthDay
  int RunTime{}; // format HoursMinutes
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
  double RunBaseline[TofRunParameters::nFebsMax][TofRunParameters::nSampicsPerFeb] {}; // V
  double RunPostTrig[TofRunParameters::nFebsMax][TofRunParameters::nSampicsPerFeb] {};
  double RunTrigThr[TofRunParameters::nChannels] {};
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