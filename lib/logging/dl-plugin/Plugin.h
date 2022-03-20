// Copyright (c) 2021  Lukasz Chodyla
// Distributed under the MIT License.
// See accompanying file LICENSE.txt for the full license.

#ifndef PLUGIN_H
#define PLUGIN_H

#include <cppps/dl/IPlugin.h>
#include <cppps/logging/Logging.h>

class Plugin: public cppps::IPlugin
{
public:
  std::string getName() const override {return PLUGIN_NAME;}
  std::string getVersionString() const override {return PLUGIN_VERSION;}

  void prepare(const cppps::ICliPtr& cli, cppps::IApplication&) override;

  void submitProviders(const cppps::SubmitProvider& submitProvider) override;
  void submitConsumers(const cppps::SubmitConsumer& /*submitConsumer*/) override {};
  void initialize() override;
  void start() override {};
  void stop() override;
  void unload() override {}

private:
  cppps::LoggerPtr sharedLogger {nullptr};
  cppps::LoggerSettings loggerSettings;
};


#endif // PLUGIN_H
