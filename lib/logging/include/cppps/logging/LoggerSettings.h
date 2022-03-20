// Copyright (c) 2021  Lukasz Chodyla
// Distributed under the MIT License.
// See accompanying file LICENSE.txt for the full license.

#ifndef LOGGERSETTINGS_H
#define LOGGERSETTINGS_H

#include <string>
#include <cstdint>

namespace cppps {

struct LoggerSettings {
  bool noStdOut = false;
  std::string logFile = "";
  uintmax_t maxLogFileSizeKB {5 * 1024}; // 5MB
  int flushThreshold {1};
  int verbosity {0};
  bool debug = false;
  int subsecondPrecision {4};
};

} // namespace cppps



#endif // LOGGERSETTINGS_H
