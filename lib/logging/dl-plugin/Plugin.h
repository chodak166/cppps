// Copyright (c) 2021  Lukasz Chodyla
// Distributed under the Boost Software License v1.0.
// See accompanying file LICENSE.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt for the full license.

#ifndef PLUGIN_H
#define PLUGIN_H

#include <cppps/dl/IPlugin.h>
#include <cppps/logging/Logging.h>

class Plugin: public cppps::dl::IPlugin
{
public:
  std::string getName() const override {return PLUGIN_NAME;}
  std::string getVersionString() const override {return PLUGIN_VERSION;}

  void prepare(const cppps::dl::ICliPtr& cli, cppps::dl::IApplication&) override;

  void submitProviders(const cppps::dl::SubmitProvider& submitProvider) override;
  void submitConsumers(const cppps::dl::SubmitConsumer& /*submitConsumer*/) override {};
  void initialize() override;
  void start() override {};
  void stop() override;
  void unload() override {}

private:
  cppps::LoggerPtr sharedLogger {nullptr};
  cppps::LoggerSettings loggerSettings;
};


#endif // PLUGIN_H
