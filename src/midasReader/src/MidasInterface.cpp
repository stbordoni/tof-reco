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
  LogTrace << __METHOD_NAME__ << std::endl;

  _currentEntry_ = -1;
  _midasReaderInterface_->Close();

  LogInfo << "Reopening file..." << std::endl;
  _midasReaderInterface_ = std::shared_ptr<TMReaderInterface>( TMNewReader( _filePath_.c_str() ) );
}
bool MidasInterface::isEventValid(TMEvent *eventPtr_) const {
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

template<> std::vector<int> MidasInterface::getBankDataArray(const std::string& bankName_){
  LogThrowIf( _currentEntry_ < 0, "No event loaded" );
  LogThrowIf( not this->isEventValid( this->getCurrentEvent() ), "event is not valid" );
  std::vector<int> out;

  auto* bank = _currentEvent_->FindBank( bankName_.c_str() );
  LogThrowIf( bank == nullptr, "Could not find bank with name " << bankName_ );
  LogThrowIf( bank->type != TID_INT, "Bank type id is not an INT" );

  out = GenericToolbox::getTypedArray<int>(bank->data_size, _currentEvent_->GetBankData( bank ));
  return out;
}


