// Copyright (c) 2021  Lukasz Chodyla
// Distributed under the MIT License.
// See accompanying file LICENSE.txt for the full license.
 
#include "cppps/logging/Logging.h"

#include <easylogging++.h>
#include <filesystem>
#include <iostream>

INITIALIZE_NULL_EASYLOGGINGPP

using cppps::LoggerPtr;

namespace {

void rolloutHandler(const char *filename, std::size_t size);

}

namespace cppps {

class Logger
{
public:
  Logger(const el::base::type::StoragePointer& storage)
    : storage{storage} {}
  el::base::type::StoragePointer getStorage() {return storage;}
private:
  el::base::type::StoragePointer storage;
};

} // namespace cppps

LoggerPtr cppps::setupLogger(const LoggerSettings& settings)
{
  auto storage = el::Helpers::storage();
  if (!storage) {
    storage = std::make_shared<el::base::Storage>(el::LogBuilderPtr(
                                  new el::base::DefaultLogBuilder()));
    el::Helpers::setStorage(storage);
  }

  el::Configurations elConfig;
  elConfig.setToDefault();

  elConfig.setGlobally(
        el::ConfigurationType::Format, "%datetime [%level]: %msg");
  elConfig.setGlobally(
        el::ConfigurationType::ToStandardOutput, settings.noStdOut ? "false" : "true");
  elConfig.setGlobally(
        el::ConfigurationType::PerformanceTracking, "false");
  elConfig.setGlobally(
        el::ConfigurationType::SubsecondPrecision, std::to_string(settings.subsecondPrecision));
  elConfig.setGlobally(
        el::ConfigurationType::MaxLogFileSize, std::to_string(settings.maxLogFileSizeKB * 1024));
  elConfig.setGlobally(
        el::ConfigurationType::LogFlushThreshold, std::to_string(settings.flushThreshold));

  elConfig.setGlobally(
        el::ConfigurationType::ToFile, settings.logFile.empty() ? "false" : "true");
  if(!settings.logFile.empty())
    elConfig.setGlobally(
          el::ConfigurationType::Filename, settings.logFile);

  //display function name in debug logs
  elConfig.set(el::Level::Debug,
               el::ConfigurationType::Format, "%datetime [%level] (%func): %msg");

  //display log location in tace logs
  elConfig.set(el::Level::Trace,
               el::ConfigurationType::Format, "%datetime [%level] (%loc): %msg");


  elConfig.set(el::Level::Debug,
               el::ConfigurationType::ToStandardOutput, settings.debug ? "true" : "false");

  elConfig.set(el::Level::Debug, el::ConfigurationType::Enabled, settings.debug ? "true" : "false");
  elConfig.set(el::Level::Trace, el::ConfigurationType::Enabled, settings.debug ? "true" : "false");

  //handle file rolling
  el::Loggers::addFlag(el::LoggingFlag::StrictLogFileSizeCheck);
  el::Helpers::installPreRollOutCallback(rolloutHandler);

  el::Loggers::setVerboseLevel(settings.verbosity);
  el::Loggers::reconfigureLogger("default", elConfig);

  return std::make_shared<Logger>(el::Helpers::storage());
}

void cppps::importLogger(const LoggerPtr& logger)
{
  el::Helpers::setStorage(logger->getStorage());
}


// ---------

namespace {

void rolloutHandler(const char *filename, std::size_t size)
{
  std::cout << "******** Rolling out ["
            << filename
            << "] because it reached [" << size << " bytes]"
            << "********" << std::endl;

  //remove old backup (if exists)
  std::string bakFilename = std::string(filename)+".1";
  std::filesystem::remove(bakFilename);

  //backup current file
  std::filesystem::rename(filename, bakFilename);
}

}
