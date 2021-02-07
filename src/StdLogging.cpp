// Copyright (c) 2021  Lukasz Chodyla
// Distributed under the Boost Software License v1.0.
// See accompanying file LICENSE.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt for the full license.

#include "cppps/Logging.h"
#include "cppps/Cli.h"
#include "cppps/stdeasylog.h"

using cppps::Cli;

void cppps::setupLoggingCli(Cli& /*cli*/)
{
  LOG(WARNING) << "Warning: logger CLI setup ignored by the simplified logger";
}

void cppps::setupLogger()
{
  LOG(WARNING) << "Warning: logger setup ignored by the simplified logger";
}
