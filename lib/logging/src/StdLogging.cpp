// Copyright (c) 2021  Lukasz Chodyla
// Distributed under the Boost Software License v1.0.
// See accompanying file LICENSE.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt for the full license.
 
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
