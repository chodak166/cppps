// Copyright (c) 2021  Lukasz Chodyla
// Distributed under the Boost Software License v1.0.
// See accompanying file LICENSE.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt for the full license.

#ifndef LOGGINGCLI_H
#define LOGGINGCLI_H

#include "cppps/logging/LoggerSettings.h"
#include <string>
#include <cstdint>

namespace cppps {

template <class T>
void setupLoggingCli(T& cli, LoggerSettings& settings)
{
  cli.addFlag("--log-debug",        settings.debug,             "Log debug and trace messages");
  cli.addFlag("--log-nostd",        settings.noStdOut,          "Do not log to the standard output");
  cli.addOption("--log-file",       settings.logFile,           "Log file path, disabled if empty (default)");
  cli.addOption("--log-file-size",  settings.maxLogFileSizeKB,  "Max log file size (KB) until rotated");
  cli.addOption("--log-flush",      settings.flushThreshold,    "Log flush threshlod");
  cli.addOption("--log-verbosity",  settings.verbosity,         "Log verbosity level");
}

} // namespace cppps



#endif // LOGGINGCLI_H
