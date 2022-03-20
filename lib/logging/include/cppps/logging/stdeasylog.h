// Copyright (c) 2021  Lukasz Chodyla
// Distributed under the MIT License.
// See accompanying file LICENSE.txt for the full license.

/* This is simple logging helper mimicking some Easylogging++ macros
 * to be used in the early development stage.
 *
 * Usage:
 *
 * LOG(INFO) << "Info log with " << std::hex << 254 << " hex number";
 * LOG(WARNING) << "Caution!";
 * LOG(ERROR) << "Error log using std::cerr";
 * VLOG(9) << "Verbose log";
 * LOG(DEBUG) << "Debug log with predefined [source:function:line] format";
 *
 * Please note that verbosity level in VLOG macro is ignored, so all
 * verbose messages will be printed.
 */

#ifndef STDEASYLOG_H
#define STDEASYLOG_H

#include <iostream>
#include <chrono>
#include <sstream>
#include <iomanip>

namespace stdeasylog
{

struct LogStarter
{
  std::ostream& stream = std::cout;
  LogStarter() = default;
  LogStarter(std::ostream& customStream): stream{customStream}{}
};

struct Log
{
  bool endSequence{true};
  std::ostream& stream;
  Log(): stream(std::cout) { }
  Log(std::ostream& customStream): stream(customStream) { }
  Log(Log&& rhs):  stream(rhs.stream) { rhs.endSequence = false; }
  Log(Log const &) = delete;
  ~Log() { if (endSequence) stream << std::endl; }
};

template <typename T>
Log operator<<(Log&& log, T const& value)
{
  log.stream << value;
  return std::move(log);
}

template <typename T>
Log operator<<(LogStarter&& starter, T const& value)
{
  return std::move(Log(starter.stream) << value);
}

inline std::string getStdLogTimeString()
{
  using namespace std::chrono;

  auto now = system_clock::now();
  auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;
  auto in_time_t = system_clock::to_time_t(now);
  const auto format = "%Y-%m-%d %X," + std::to_string(ms.count()) + " [STDLOG]";

  std::stringstream ss;
  ss << std::put_time(std::localtime(&in_time_t), format.data());
  return ss.str();
}

}

#define INFO    stdeasylog::LogStarter()           << stdeasylog::getStdLogTimeString() << " INFO: "
#define ERROR   stdeasylog::LogStarter(std::cerr)  << stdeasylog::getStdLogTimeString() << " ERROR: "
#define WARNING stdeasylog::LogStarter()           << stdeasylog::getStdLogTimeString() << " WARNING: "
#define VERBOSE stdeasylog::LogStarter()           << stdeasylog::getStdLogTimeString() << " VERB: "
#define DEBUG   stdeasylog::LogStarter()           << stdeasylog::getStdLogTimeString() << " DEBUG[" \
                                                   << __FILE__ << ":" << __FUNCTION__ \
                                                   << ":" << __LINE__ << "]: "

#define LOG(LOGLEVEL)   LOGLEVEL
#define VLOG(n)         VERBOSE


#endif // STDEASYLOG_H
