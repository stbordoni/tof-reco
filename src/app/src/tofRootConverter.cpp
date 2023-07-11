//
// Created by Adrien Blanchet on 11/07/2023.
//

#include "MidasInterface.h"

#include "CmdLineParser.h"
#include "GenericToolbox.RawDataArray.h"
#include "Logger.h"

#include "SAMPIC_256Ch_Type.h"

#include "TFile.h"
#include "TTree.h"


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

  std::string outFilePath = clp.getOptionVal<std::string>("midasFile") + ".root";
  LogInfo << "Output file will be writen as: \"" << outFilePath << "\"" << std::endl;

  auto outFile = std::make_unique<TFile>( outFilePath.c_str(), "RECREATE" );
  auto* outTree = new TTree( "TofHit", "TofHit" );

  int midasEntryIndex;
  int hitIndex;
  HitStruct hitBuffer;

  outTree->Branch( "midasEntryIndex", &midasEntryIndex );
  outTree->Branch( "hitIndex", &hitIndex );

  outTree->Branch( "HitNumber", &hitBuffer.HitNumber );
  outTree->Branch( "ChannelIndex", &hitBuffer.ChannelIndex );
  outTree->Branch( "FeBoardIndex", &hitBuffer.FeBoardIndex );
  outTree->Branch( "ChannelIndex", &hitBuffer.ChannelIndex );
  outTree->Branch( "ChannelIndex", &hitBuffer.ChannelIndex );
  outTree->Branch( "SampicIndex", &hitBuffer.SampicIndex );

  outTree->Branch( "TOTValue", &hitBuffer.TOTValue );
  outTree->Branch( "FirstCellTimeStamp", &hitBuffer.FirstCellTimeStamp );

  outTree->Branch( "CorrectedDataSamples", hitBuffer.CorrectedDataSamples, ("CorrectedDataSamples[" + std::to_string(MAX_NB_OF_SAMPLES) + "]/F").c_str() );
  outTree->Branch( "RawDataSamples", hitBuffer.RawDataSamples, ("RawDataSamples[" + std::to_string(MAX_NB_OF_SAMPLES) + "]/s").c_str() );
  outTree->Branch( "OrderedRawDataSamples", hitBuffer.OrderedRawDataSamples, ("OrderedRawDataSamples[" + std::to_string(MAX_NB_OF_SAMPLES) + "]/s").c_str() );


  long nOffset = clp.getOptionVal("nOffset", long(0));
  long nEventToPrint = clp.getOptionVal("nEventToPrint", long(-1));

  LogWarningIf( nOffset != 0 ) << GET_VAR_NAME_VALUE(nOffset) << std::endl;
  LogWarningIf( nEventToPrint != -1 ) << GET_VAR_NAME_VALUE(nEventToPrint) << std::endl;

  LogWarning << "Fetching nb of entries in file..." << std::endl;
  long nEntries = midasInterface.fetchNbEvents();
  LogInfo << "File has " << nEntries << " recorded SAMPIC events." << std::endl;

  for(long iEntry = 0 ; iEntry < nEntries ; iEntry++){

    GenericToolbox::displayProgressBar(iEntry, nEntries, LogWarning.getPrefixString() + "Loading MIDAS entries...");
    auto* entry = midasInterface.getEntry(iEntry);

    if( iEntry < nOffset ){ LogDebug << GET_VAR_NAME_VALUE(iEntry) << std::endl; continue; }
    if( nEventToPrint != -1 and iEntry - nOffset < nEventToPrint ){ break; }

    entry->FindAllBanks(); // fetch banks, otherwise give 0

    if( not MidasInterface::isEventValid(entry) ){
      LogAlert << "Event #" << iEntry << " contains an error." << std::endl;
      LogAlert << "If it's Event 0, it's normal since the first MIDAS event containing other info." << std::endl;
      continue;
    }

    midasInterface.fillSampicEvent();

    auto* sampicEv = &midasInterface.getSampicEventBuffer();
    for( int iHit=0 ; iHit < sampicEv->NbOfHitsInEvent ; iHit++ ){
      midasEntryIndex = int(iEntry);
      hitIndex = iHit;

      hitBuffer = sampicEv->Hit[iHit];

      outTree->Fill();
    }

  }

  outTree->Write();
  outFile->Close();

  LogInfo << "Output file writen as: \"" << outFilePath << "\"" << std::endl;

}