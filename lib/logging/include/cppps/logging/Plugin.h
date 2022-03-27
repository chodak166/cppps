// Copyright (c) 2021  Lukasz Chodyla
// Distributed under the MIT License.
// See accompanying file LICENSE.txt for the full license.

#ifndef PLUGIN_H
#define PLUGIN_H

#include <cppps/dl/IPlugin.h>
#include <cppps/dl/ICli.h>

#include <cppps/logging/Logging.h>
#include <cppps/logging/LoggingCli.h>

#ifndef CPPPS_LOGGING_SL_NAME
#  define CPPPS_LOGGING_SL_NAME "shared_logger"
#endif

class Plugin: public cppps::IPlugin
{
public:
  std::string getName() const override {return PLUGIN_NAME;}
  std::string getVersionString() const override {return PLUGIN_VERSION;}

  void prepare(const cppps::ICliPtr& cli, cppps::IApplication&) override {
    cppps::setupLoggingCli(*cli, loggerSettings);
  }

  void submitProviders(const cppps::SubmitProvider& submitProvider) override {
    submitProvider(CPPPS_LOGGING_SL_NAME, [this](){return sharedLogger;});
  }

  void submitConsumers(const cppps::SubmitConsumer& /*submitConsumer*/) override {};

  void initialize() override {
    sharedLogger = cppps::setupLogger(loggerSettings);
  }

  void start() override {};
  void stop() override {};
  void unload() override {}

private:
  cppps::LoggerPtr sharedLogger {nullptr};
  cppps::LoggerSettings loggerSettings;
};

#endif // PLUGIN_H
