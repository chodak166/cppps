// Copyright (c) 2021  Lukasz Chodyla
// Distributed under the MIT License.
// See accompanying file LICENSE.txt for the full license.
 
#include "cppps/logging/Logging.h"

using cppps::LoggerPtr;

namespace cppps {

class Logger{};

} // namespace cppps

LoggerPtr cppps::setupLogger(const cppps::LoggerSettings&)
{
  return nullptr;
}

void cppps::importLogger(const LoggerPtr&)
{
  // empty
}
