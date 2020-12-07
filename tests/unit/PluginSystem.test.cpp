// Copyright (c) 2020  Lukasz Chodyla
// Distributed under the Boost Software License v1.0.
// See accompanying file LICENSE.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt for the full license.

#include "cppps/ICli.h"
#include "cppps/IPlugin.h"
#include "cppps/exceptions.h"
#include "cppps/PluginSystem.h"
#include "cppps/Resource.h"

#include <catch2/catch.hpp>
#include <fakeit/single_header/catch/fakeit.hpp>

#include <map>
#include <string_view>
#include <vector>
#include <functional>
#include <any>
#include <list>

using namespace fakeit;
using namespace cppps;


struct ProductA
{
  ProductA(std::string_view name): value{name} {}
  std::string value;
};

using ProductAPtr = std::shared_ptr<ProductA>;

namespace test {
namespace {

constexpr auto PLUGIN_A_NAME = "A";
constexpr auto PLUGIN_B_NAME = "B";
constexpr auto PLUGIN_C_NAME = "C";
constexpr auto PRODUCT_A_KEY = "product_a";
constexpr auto PRODUCT_A_VALUE = "the value of product A";
constexpr auto PRODUCT_X_KEY = "product_x";
const std::string PREPARE_TAG = "_init";
const std::string INIT_TAG = "_init";
const std::string START_TAG = "_start";
const std::string STOP_TAG = "_stop";
const std::string UNLOAD_TAG = "_unload";

struct Fixture
{
  fakeit::Mock<ICli> cli;
  fakeit::Mock<IPlugin> pluginA;
  fakeit::Mock<IPlugin> pluginB;
  fakeit::Mock<IPlugin> pluginC;

  ICliPtr cliPtr = cli.getFakePtr();

  PluginSystem::LoadedPlugins plugins;
  std::vector<std::string> processedPlugins;

  ProductAPtr productAPtr {nullptr};

  Fixture()
  {
    plugins.emplace_back(IPluginDPtr(&pluginB.get(), [](auto*){}));
    plugins.emplace_back(IPluginDPtr(&pluginA.get(), [](auto*){}));


    setupLifecycleMethods(pluginA, test::PLUGIN_A_NAME);
    setupLifecycleMethods(pluginB, test::PLUGIN_B_NAME);
    setupLifecycleMethods(pluginC, test::PLUGIN_C_NAME);
    setupPluginADependencies();
    setupPluginBDependencies();
  }

  void setupLifecycleMethods(Mock<IPlugin>& plugin, std::string_view name)
  {
    When(Method(plugin, getName)).AlwaysDo([name](){return name.data();});
    When(Method(plugin, prepare)).Do([this, name](auto){
      processedPlugins.push_back(name.data() + test::PREPARE_TAG);});
    When(Method(plugin, initialize)).Do([this, name](){
      processedPlugins.push_back(name.data() + test::INIT_TAG);});
    When(Method(plugin, start)).Do([this, name](){
      processedPlugins.push_back(name.data() + test::START_TAG);});
    When(Method(plugin, stop)).Do([this, name](){
      processedPlugins.push_back(name.data() + test::STOP_TAG);});
    When(Method(plugin, unload)).Do([this, name](){
      processedPlugins.push_back(name.data() + test::UNLOAD_TAG);});
  }

  void setupPluginADependencies()
  {
    Fake(Method(pluginA, submitConsumers));
    When(Method(pluginA, submitProviders)).Do([](SubmitProvider submit) {
      auto providerA = []() {
        return std::make_shared<ProductA>(test::PRODUCT_A_VALUE);
      };
      submit(test::PRODUCT_A_KEY, providerA);
    });
  }

  void setupPluginBDependencies()
  {
    Fake(Method(pluginB, submitProviders));
    When(Method(pluginB, submitConsumers)).Do([this](SubmitConsumer submit) {
      auto consumer = [this](const Resource& res) {
        productAPtr = res.as<ProductAPtr>();
      };
      submit(test::PRODUCT_A_KEY, consumer);
    });
  }

};

} // namespace
} // namespace test

TEST_CASE_METHOD(test::Fixture, "Testing plugins initialization", "[ps_init]")
{
  PluginSystem pluginSystem(cliPtr);
  pluginSystem.mergePlugins(plugins);

  SECTION("When the preparation stage is done, then all plugins should be prepared in the order they were added")
  {
    pluginSystem.prepare();
    REQUIRE(processedPlugins.at(0) == test::PLUGIN_B_NAME + test::PREPARE_TAG);
    REQUIRE(processedPlugins.at(1) == test::PLUGIN_A_NAME + test::PREPARE_TAG);
  }

  SECTION("When the preparation stage is done, then all the plugins should get access to the command line handle")
  {
    pluginSystem.prepare();
    Verify(Method(pluginA, prepare).Using(cliPtr)).Once();
    Verify(Method(pluginB, prepare).Using(cliPtr)).Once();
  }

  SECTION("When the initialization stage is done, then all the plugins should be initialized in the dependency order")
  {
    pluginSystem.initialize();
    REQUIRE(processedPlugins.at(0) == test::PLUGIN_A_NAME + test::INIT_TAG);
    REQUIRE(processedPlugins.at(1) == test::PLUGIN_B_NAME + test::INIT_TAG);
  }

  SECTION("When the initialization stage is done, then all consumers are fed with required products")
  {
    pluginSystem.initialize();
    REQUIRE(productAPtr != nullptr);
    REQUIRE(productAPtr->value == test::PRODUCT_A_VALUE);
  }

  SECTION("When consumer requirements are not satisfied, then an exception is thrown")
  {
    Fake(Method(pluginC, submitProviders));
    When(Method(pluginC, submitConsumers)).Do([](const SubmitConsumer& submit) {
      auto consumer = [](const Resource&) {};
      submit(test::PRODUCT_X_KEY, consumer);
    });

    PluginSystem::LoadedPlugins extraPlugins;
    extraPlugins.emplace_back(IPluginDPtr(&pluginC.get(), [](auto*){}));
    pluginSystem.mergePlugins(extraPlugins);

    REQUIRE_THROWS_AS(pluginSystem.initialize(), UnresolvedDependencyException);
  }

  SECTION("When plugin dependencies create a cycle, then an exception is thrown")
  {
    When(Method(pluginA, submitConsumers)).Do([](const SubmitConsumer& submit) {
      auto consumer = [](const Resource&) {};
      submit(test::PRODUCT_A_KEY, consumer);
    });

    REQUIRE_THROWS_AS(pluginSystem.initialize(), CircularDependencyException);
  }

}


TEST_CASE_METHOD(test::Fixture, "Testing plugin life cycle", "[ps_cycle]")
{
  PluginSystem pluginSystem(cliPtr);
  pluginSystem.mergePlugins(plugins);
  pluginSystem.initialize();
  processedPlugins.clear();

  SECTION("When the starting stage is done, then all the plugins should be started in the dependency order")
  {
    pluginSystem.start();
    REQUIRE(processedPlugins.at(0) == test::PLUGIN_A_NAME + test::START_TAG);
    REQUIRE(processedPlugins.at(1) == test::PLUGIN_B_NAME + test::START_TAG);
  }

  SECTION("When the stopping stage is done, then all the plugins should be stopped in reverse dependency order")
  {
    pluginSystem.stop();
    REQUIRE(processedPlugins.at(0) == test::PLUGIN_B_NAME + test::STOP_TAG);
    REQUIRE(processedPlugins.at(1) == test::PLUGIN_A_NAME + test::STOP_TAG);
  }

  SECTION("When the unloading stage is done, then all the plugins should be unloaded in reverse dependency order")
  {
    pluginSystem.unload();
    REQUIRE(processedPlugins.at(0) == test::PLUGIN_B_NAME + test::UNLOAD_TAG);
    REQUIRE(processedPlugins.at(1) == test::PLUGIN_A_NAME + test::UNLOAD_TAG);
  }

  //TODO: should the unload() method be called after an exception is thrown?

}


namespace test {
namespace {


} // namespace
} // namespace test

