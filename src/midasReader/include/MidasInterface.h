//
// Created by Adrien Blanchet on 31/05/2023.
//

#ifndef TOF_MIDAS_DATA_READER_MIDASFILEINTERFACE_H
#define TOF_MIDAS_DATA_READER_MIDASFILEINTERFACE_H


#include "midasio.h"

#include <string>
#include <memory>


class MidasInterface {

public:
  MidasInterface() = default;

  // setters
  void setFilePath(const std::string &filePath_);

  // getters
  [[nodiscard]] long getCurrentEntry() const;
  [[nodiscard]] const TMEvent* getCurrentEvent() const;
  TMEvent* getCurrentEvent();

  // init
  void initialize();

  // core
  void rewind();
  bool isEventValid(TMEvent* eventPtr_) const;
  long fetchNbEvents();
  TMEvent* getEntry(long iEntry_);
  TMEvent* nextEvent();

  template<typename T> std::vector<T> getBankDataArray(const std::string& bankName_);

private:
  // parameters
  std::string _filePath_{};

  // internals
  bool _isInitialized_{false};
  long _currentEntry_{-1};
  std::shared_ptr<TMReaderInterface> _midasReaderInterface_{nullptr};
  std::unique_ptr<TMEvent> _currentEvent_{nullptr};

};


#endif //TOF_MIDAS_DATA_READER_MIDASFILEINTERFACE_H
