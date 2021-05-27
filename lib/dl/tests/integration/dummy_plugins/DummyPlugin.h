// Copyright (c) 2020  Lukasz Chodyla
// Distributed under the Boost Software License v1.0.
// See accompanying file LICENSE.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt for the full license.

#ifndef DUMMYPLUGIN_H
#define DUMMYPLUGIN_H

#include "cppps/dl/IPlugin.h"

class DummyPlugin: public cppps::IPlugin
{
public:
  DummyPlugin() = default;
  DummyPlugin(std::string_view name, std::string_view version)
    : name {name}, version {version} {}
  ~DummyPlugin() = default;

  // IPlugin interface
  std::string getName() const override {return name;}
  std::string getVersionString() const override {return version;}
  void prepare(const cppps::ICliPtr&, cppps::IApplication&) override {};
  void submitProviders(const cppps::SubmitProvider&) override {};
  void submitConsumers(const cppps::SubmitConsumer&) override {};
  void initialize() override {}
  void start() override {}
  void stop() override {}
  void unload() override {}
private:
  std::string name {"unnamed plugin"};
  std::string version {"0.0.1"};

};

#endif // DUMMYPLUGIN_H
