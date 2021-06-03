// Copyright (c) 2021  Lukasz Chodyla
// Distributed under the Boost Software License v1.0.
// See accompanying file LICENSE.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt for the full license.

#include "Plugin.h"

#include <cppps/dl/ICli.h>
#include <cppps/logging/Logging.h>
#include <cppps/logging/LoggingCli.h>

#include <boost/dll/alias.hpp>
#include <iostream>

using namespace cppps::dl;

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

// --- export the plugin ---

cppps::dl::IPluginUPtr makePlugin()
{
  return std::make_unique<Plugin>();
}

BOOST_DLL_ALIAS(makePlugin, make_plugin)
