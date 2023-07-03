//
// Created by C. Jesus-Valls on Jul 3rd, 2023. 
// cesar.jesus@cern.ch
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
  // thisRun.RunGenerateOutputFile(output_directory);

  // read AnalysisSettings.json
  std::string RunAnalysisSettingsFile = "../../../AnalysisSettings.json"; 
    std::ifstream RunAnalysisSettingsStream(RunAnalysisSettingsFile.c_str());
    // if (RunAnalysisSettingsStream.good()) RunSelectedAnalysisOptions = true;
  
    if (!RunAnalysisSettingsStream.is_open()) {
        std::string this_error = "Failed to open " + RunAnalysisSettingsFile;
        // return;
    }
    LogInfo << "Reading analysis settings from " << RunAnalysisSettingsFile << std::endl;
    
    nlohmann::json analysis_settings_file;
    RunAnalysisSettingsStream >> analysis_settings_file;
    bool waveform_display = analysis_settings_file["HitDisplay"];
    bool run_root_app = analysis_settings_file["PlotsDisplay"];
    
  // bool waveform_display = false;
  LogInfo << "Run number " << thisRun.GetRunNumber() << std::endl;

  std::vector <std::string> PlaneLabels = {"U","D","T","B","L","R"}; // use map instead!
  std::map<std::string, int> PlaneNumbers;
  for (int i= 0; i < 6; i++) PlaneNumbers[PlaneLabels[i]] = i;

  int event_counter = 0;
  for (auto  eventit : thisRun.GetRunEventsList()) {
    std::map <int, int> plane_hits;
    for (auto  signalit : eventit.GetEventSignalsList()) {
      if (signalit.GetSignalType() != 3) continue;
      if (signalit.GetSignalHitMin().GetHitPlane() == -1) continue;
      
        plane_hits[signalit.GetSignalHitMin().GetHitPlane()]=1;
    }
    //std::cout << "different planes size: " << plane_hits.size() << std::endl;

    // "U" | "B" | "R" | "L"  | "T" | "D"  
    // 0,3,5,4,2,1
    if(plane_hits.size()>2){
      int ordered_planes [6] = {0,3,5,4,2,1};
      std::cout << "-- analyzing event --" << std::endl;
      for (int p_idx=0; p_idx<6; p_idx++){
        for (auto  signalit : eventit.GetEventSignalsList()) {
          if (signalit.GetSignalType() != 3) continue;
          if (signalit.GetSignalHitMin().GetHitPlane() != ordered_planes[p_idx]) continue;
          std::cout << PlaneLabels[signalit.GetSignalHitMin().GetHitPlane()] << ", " << signalit.GetSignalHitMin().GetHitBar() << ", "
          << signalit.GetSignalPosition()  << ", " << signalit.GetSignalHitMin().GetHitPeakSample() << std::endl;//<< ", " << signalit.GetSignalTime()*1.E-9 << std::endl;
        

        }
      }
    }

    event_counter++;
  }

  return 0;
}