//
// Created by Adrien Blanchet on 31/05/2023.
//

#ifndef TOF_MIDAS_DATA_READER_MIDASFILEINTERFACE_H
#define TOF_MIDAS_DATA_READER_MIDASFILEINTERFACE_H


#include "midasio.h"
#include "SAMPIC_256Ch_Type.h"

#include "Logger.h"
#include "GenericToolbox.h"

#include <string>
#include <memory>
#include <vector>


class MidasInterface {

public:
  MidasInterface() = default;

  // setters
  void setFilePath(const std::string &filePath_);

  // getters
  [[nodiscard]] long getCurrentEntry() const;
  [[nodiscard]] const TMEvent* getCurrentEvent() const;
  [[nodiscard]] const EventStruct &getSampicEventBuffer() const;
  TMEvent* getCurrentEvent();


  // init
  void initialize();

  // core
  void rewind();
  void fillSampicEvent();
  void printSampicEvent() const;
  long fetchNbEvents();
  TMEvent* getEntry(long iEntry_);
  TMEvent* nextEvent();

  template<typename T> std::vector<T> getBankDataArray(const std::string& bankName_);

  static bool isEventValid(TMEvent* eventPtr_) ;
  template<typename T> static unsigned int getTid();

private:
  // parameters
  std::string _filePath_{};

  // internals
  bool _isInitialized_{false};
  long _currentEntry_{-1};
  std::shared_ptr<TMReaderInterface> _midasReaderInterface_{nullptr};
  std::unique_ptr<TMEvent> _currentEvent_{nullptr};

  EventStruct _sampicEventBuffer_{};

};


// template need to be defined in header
template<typename T> std::vector<T> MidasInterface::getBankDataArray(const std::string& bankName_){
  LogThrowIf( _currentEntry_ < 0, "No event loaded" );
  LogThrowIf( not this->isEventValid( this->getCurrentEvent() ), "event is not valid" );
  auto* bank = _currentEvent_->FindBank( bankName_.c_str() );

  // return empty if non-existing
  if( bank == nullptr ){ return std::vector<T>(); }

  LogThrowIf( bank->type != MidasInterface::getTid<T>(), bankName_ << ": bank type id (" << bank->type << ") is not matching to provided type." );

  return GenericToolbox::getTypedArray<T>(bank->data_size, _currentEvent_->GetBankData( bank ));
}

template<typename T> unsigned int MidasInterface::getTid(){
  if     ( std::is_same<T, bool>::value )           return TID_BOOL;
  else if( std::is_same<T, char>::value )           return TID_INT8;
  else if( std::is_same<T, short>::value )          return TID_INT16;
  else if( std::is_same<T, int>::value )            return TID_INT32;
  else if( std::is_same<T, long>::value )           return TID_INT64;
  else if( std::is_same<T, unsigned char>::value )  return TID_UINT8;
  else if( std::is_same<T, unsigned short>::value ) return TID_UINT16;
  else if( std::is_same<T, unsigned int>::value )   return TID_UINT32;
  else if( std::is_same<T, unsigned long>::value )  return TID_UINT64;
  else if( std::is_same<T, float>::value )          return TID_FLOAT32;
  else if( std::is_same<T, double>::value )         return TID_FLOAT64;
}



#endif //TOF_MIDAS_DATA_READER_MIDASFILEINTERFACE_H
