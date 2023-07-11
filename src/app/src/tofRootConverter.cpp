//
// Created by Adrien Blanchet on 11/07/2023.
//

#include "MidasInterface.h"

#include "CmdLineParser.h"
#include "Logger.h"


LoggerInit([]{
  Logger::getUserHeader() << "[tofRootConverter.cpp]";
});

int main(int argc, char *argv[]){

  CmdLineParser clp;
  clp.getDescription() << "> printTofData is a tool app for print the content of Midas files"<< std::endl;

  clp.addDummyOption("Main options");
  clp.addOption("midasFile", {"-f", "--file"}, "Specify path to Midas file");
  clp.addOption("nEventToPrint", {"-n"}, "Number of events to print");
  clp.addOption("nOffset", {"--offset"}, "Start from a given event");

  // usage allways displayed
  LogInfo << clp.getDescription().str() << std::endl;

  LogInfo << "Usage: " << std::endl;
  LogInfo << clp.getConfigSummary() << std::endl << std::endl;

  clp.parseCmdLine(argc, argv);

  LogThrowIf( clp.isNoOptionTriggered(), "No option was provided." );

  LogInfo << "Provided arguments: " << std::endl;
  LogInfo << clp.getValueSummary() << std::endl << std::endl;

  LogThrowIf(not clp.isOptionTriggered("midasFile"), "No midas file provided.");

  LogWarning << "Opening \"" << clp.getOptionVal<std::string>("midasFile") << "\"" << std::endl;
  MidasInterface midasInterface{};
  midasInterface.setFilePath( clp.getOptionVal<std::string>("midasFile") );

  LogWarning << "Initializing MIDAS interface..." << std::endl;
  midasInterface.initialize();

  long nOffset = clp.getOptionVal("nOffset", long(0));
  long nEventToPrint = clp.getOptionVal("nEventToPrint", long(-1));

  LogWarningIf( nOffset != 0 ) << GET_VAR_NAME_VALUE(nOffset) << std::endl;
  LogWarningIf( nEventToPrint != -1 ) << GET_VAR_NAME_VALUE(nEventToPrint) << std::endl;

  LogWarning << "Fetching nb of entries in file..." << std::endl;
  long nEntries = midasInterface.fetchNbEvents();
  LogInfo << "File has " << nEntries << " recorded SAMPIC events." << std::endl;

  for(long iEntry = 0 ; iEntry < nEntries ; iEntry++){

    auto* entry = midasInterface.getEntry(iEntry);

    if( iEntry < nOffset ){ LogDebug << GET_VAR_NAME_VALUE(iEntry) << std::endl; continue; }
    if( nEventToPrint != -1 and iEntry - nOffset < nEventToPrint ){ break; }

    entry->FindAllBanks(); // fetch banks, otherwise give 0

    LogInfo << "--------------------" << std::endl;
    LogInfo << "Event #" << iEntry << std::endl;
    LogScopeIndent;
    entry->PrintHeader();
    entry->PrintBanks();

    if( not MidasInterface::isEventValid(entry) ){
      LogAlert << "Event #" << iEntry << " contains an error." << std::endl;
      LogAlert << "If it's Event 0, it's normal since the first MIDAS event containing other info." << std::endl;
      continue;
    }

    midasInterface.fillSampicEvent();
    midasInterface.printSampicEvent();

  }

}