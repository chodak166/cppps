// Copyright (c) 2021  Lukasz Chodyla
// Distributed under the Boost Software License v1.0.
// See accompanying file LICENSE.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt for the full license.

#include "cppps/Logging.h"
#include "cppps/ICli.h"

#include <easylogging++.h>
#include <filesystem>
#include <iostream>

INITIALIZE_EASYLOGGINGPP

using cppps::ICli;

namespace {

bool debug {false};
std::string logFile;
uintmax_t maxLogFileSizeKB {5 * 1024}; // 5MB
int flushThreshold {1};
int subsecondPrecision {4};
int verbosity {0};
bool noStdOut {false};

void rolloutHandler(const char *filename, std::size_t size);

}

void cppps::setupLoggingCli(ICli& cli)
{
  cli.addFlag("--log-debug", debug, "Log debug and trace messages");
  cli.addFlag("--log-nostd", noStdOut, "Do not log to the standard output");
  cli.addOption("--log-file", logFile, "Log file path, disabled if empty (default)");
  cli.addOption("--log-file-size", maxLogFileSizeKB, "Max log file size (KB) until rotated");
  cli.addOption("--log-flush", flushThreshold, "Log flush threshlod");
  cli.addOption("--log-verbosity", verbosity, "Log verbosity level");
}

void cppps::setupLogger()
{
  el::Configurations elConfig;
  elConfig.setToDefault();

  elConfig.setGlobally(
        el::ConfigurationType::Format, "%datetime [%level]: %msg");
  elConfig.setGlobally(
        el::ConfigurationType::ToStandardOutput, noStdOut ? "false" : "true");
  elConfig.setGlobally(
        el::ConfigurationType::PerformanceTracking, "false");
  elConfig.setGlobally(
        el::ConfigurationType::SubsecondPrecision, std::to_string(subsecondPrecision));
  elConfig.setGlobally(
        el::ConfigurationType::MaxLogFileSize, std::to_string(maxLogFileSizeKB * 1024));
  elConfig.setGlobally(
        el::ConfigurationType::LogFlushThreshold, std::to_string(flushThreshold));

  elConfig.setGlobally(
        el::ConfigurationType::ToFile, logFile.empty() ? "false" : "true");
  if(!logFile.empty())
    elConfig.setGlobally(
          el::ConfigurationType::Filename, logFile);

  //display function name in debug logs
  elConfig.set(el::Level::Debug,
               el::ConfigurationType::Format, "%datetime [%level] (%func): %msg");

  //display log location in tace logs
  elConfig.set(el::Level::Trace,
               el::ConfigurationType::Format, "%datetime [%level] (%loc): %msg");


  elConfig.set(el::Level::Debug, el::ConfigurationType::Enabled, debug ? "true" : "false");
  elConfig.set(el::Level::Trace, el::ConfigurationType::Enabled, debug ? "true" : "false");

  //handle file rolling
  el::Loggers::addFlag(el::LoggingFlag::StrictLogFileSizeCheck);
  el::Helpers::installPreRollOutCallback(rolloutHandler);

  el::Loggers::setVerboseLevel(verbosity);
  el::Loggers::reconfigureLogger("default", elConfig);
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
