//
// Created by Adrien Blanchet on 31/05/2023.
//

#include "MidasInterface.h"

#include "Logger.h"
#include "GenericToolbox.h"

#include "midasio.h"


LoggerInit([]{
  Logger::getUserHeader() << "[" << FILENAME << "]";
});

// setters
void MidasInterface::setFilePath(const std::string &filePath_) {
  _filePath_ = filePath_;
}

// getters
long MidasInterface::getCurrentEntry() const {
  return _currentEntry_;
}
const TMEvent* MidasInterface::getCurrentEvent() const{
  return _currentEvent_.get();
}
const EventStruct &MidasInterface::getSampicEventBuffer() const {
  return _sampicEventBuffer_;
}

// non-const getters
TMEvent* MidasInterface::getCurrentEvent(){
  return _currentEvent_.get();
}

// init
void MidasInterface::initialize() {
  LogThrowIf(_filePath_.empty(), "file path not provided");
  LogWarning << "Opening file: " << _filePath_ << std::endl;

  _midasReaderInterface_ = std::shared_ptr<TMReaderInterface>( TMNewReader( _filePath_.c_str() ) );
  LogThrowIf(_midasReaderInterface_ == nullptr or _midasReaderInterface_->fError, "Could not open file: " << _filePath_);

  LogInfo << "Successfully opened file: " << _filePath_ << std::endl;
  _isInitialized_ = true;
}

// core
void MidasInterface::rewind(){
  LogThrowIf(not _isInitialized_);
  LogDebug << __METHOD_NAME__ << std::endl;

  _currentEntry_ = -1;
  _midasReaderInterface_->Close();

  LogInfo << "Reopening " << _filePath_ << "..." << std::endl;
  _midasReaderInterface_ = std::shared_ptr<TMReaderInterface>( TMNewReader( _filePath_.c_str() ) );
  LogInfo << "File reopened." << std::endl;
}
void MidasInterface::fillSampicEvent(){

  if( this->getBankDataArray<int>("TFNH").empty() ){
    this->getCurrentEvent()->PrintBanks();
    LogThrow("Event don't have the expected TFNH bank.");
  }

  // array of [NbOfHitsInEvent]
  auto boardIndices = this->getBankDataArray<int>("TFBI");
  auto channelIndices = this->getBankDataArray<int>("TFCI");
  auto hitNumberList = this->getBankDataArray<int>("TFHN");
  auto sampicIndexList = this->getBankDataArray<int>("TFSI");
  auto channelIndexList = this->getBankDataArray<int>("TFCI");
  auto FirstCellTimeList = this->getBankDataArray<double>("TFC0");
  auto TOTValueList = this->getBankDataArray<float>("TFTV");

  // array of [NbOfHitsInEvent][nSamples]
  auto rawSampleList = this->getBankDataArray<unsigned short>("TFRS");
  auto orderedSampleList = this->getBankDataArray<unsigned short>("TFOS");
  auto correctedSampleList = this->getBankDataArray<float>("TFCS");

  // filling struct
  _sampicEventBuffer_.NbOfHitsInEvent = this->getBankDataArray<int>("TFNH")[0];

  int sampleGlobalIndex{-1};
  for( int iHit = 0 ; iHit < _sampicEventBuffer_.NbOfHitsInEvent ; iHit++ ){

    if( not boardIndices.empty() ) _sampicEventBuffer_.Hit[iHit].FeBoardIndex = boardIndices[iHit];
    if( not channelIndices.empty() ) _sampicEventBuffer_.Hit[iHit].Channel = channelIndices[iHit];
    if( not hitNumberList.empty() ) _sampicEventBuffer_.Hit[iHit].HitNumber = hitNumberList[iHit];
    if( not sampicIndexList.empty() ) _sampicEventBuffer_.Hit[iHit].SampicIndex = sampicIndexList[iHit];
    if( not channelIndexList.empty() ) _sampicEventBuffer_.Hit[iHit].ChannelIndex = channelIndexList[iHit];
    if( not FirstCellTimeList.empty() ) _sampicEventBuffer_.Hit[iHit].FirstCellTimeStamp = FirstCellTimeList[iHit];
    if( not TOTValueList.empty() ) _sampicEventBuffer_.Hit[iHit].TOTValue = TOTValueList[iHit];

    for( int iSample = 0 ; iSample < MAX_NB_OF_SAMPLES ; iSample++ ){
      sampleGlobalIndex++;
      if( not rawSampleList.empty() ){ _sampicEventBuffer_.Hit[iHit].RawDataSamples[iSample] = rawSampleList[sampleGlobalIndex]; }
      if( not orderedSampleList.empty() ){ _sampicEventBuffer_.Hit[iHit].OrderedRawDataSamples[iSample] = orderedSampleList[sampleGlobalIndex]; }
      if( not correctedSampleList.empty() ){ _sampicEventBuffer_.Hit[iHit].CorrectedDataSamples[iSample] = correctedSampleList[sampleGlobalIndex]; }
    }

  }


}
bool MidasInterface::isEventValid(TMEvent *eventPtr_) {
  if( eventPtr_ == nullptr ){ return false; }
  if( eventPtr_->error ){
    LogAlert << "Event " << eventPtr_ << " contains an error." << std::endl;
    return false;
  }
  return true;
}
long MidasInterface::fetchNbEvents() {
  LogThrowIf(not _isInitialized_);
  long outNbEvents{-1};

  this->rewind();

  do {
    if( this->isEventValid( this->nextEvent() ) ){ outNbEvents++; }
  }
  while( _currentEvent_ != nullptr );

  return outNbEvents;
}
TMEvent* MidasInterface::getEntry(long iEntry_){
  if( iEntry_ < _currentEntry_ ){ this->rewind(); }

  while( _currentEntry_ != iEntry_ ){ nextEvent(); }
  return this->getCurrentEvent();
}
TMEvent* MidasInterface::nextEvent() {
  _currentEntry_++;
  _currentEvent_ = std::unique_ptr<TMEvent>( TMReadEvent(_midasReaderInterface_.get()) );
  return this->getCurrentEvent();
}

