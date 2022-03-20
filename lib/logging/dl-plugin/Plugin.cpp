// Copyright (c) 2021  Lukasz Chodyla
// Distributed under the MIT License.
// See accompanying file LICENSE.txt for the full license.

#include "Plugin.h"

#include <cppps/dl/ICli.h>
#include <cppps/dl/Export.h>
#include <cppps/logging/Logging.h>
#include <cppps/logging/LoggingCli.h>

#include <iostream>

using namespace cppps;

void Plugin::prepare(const ICliPtr& cli, IApplication&)
{
  cppps::setupLoggingCli(*cli, loggerSettings);
};

void Plugin::submitProviders(const SubmitProvider& submitProvider)
{
  submitProvider("shared_logger", [this](){return sharedLogger;});
};

void Plugin::initialize()
{
  sharedLogger = cppps::setupLogger(loggerSettings);
}

void Plugin::stop()
{
}


CPPPS_EXPORT_PLUGIN(Plugin)
