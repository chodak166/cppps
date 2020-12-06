// Copyright (c) 2020  Lukasz Chodyla
// Distributed under the Boost Software License v1.0.
// See accompanying file LICENSE.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt for the full license.

#include "cppps/ICli.h"
#include "cppps/IPlugin.h"
#include "cppps/Digraph.h"

#include <catch2/catch.hpp>
#include <fakeit/single_header/catch/fakeit.hpp>

#include <map>
#include <string_view>
#include <vector>
#include <functional>
#include <any>
#include <list>

//NOTE: use -Wno-disabled-macro-expansion to suppress fakeit macro warnings
using namespace fakeit;

using namespace cppps;

namespace {

}


class Resource
{
public:
  Resource() = delete;

  template<class T>
  Resource(T&& value)
  {
    object = std::move(value);
  }

  template<class T>
  T as() const
  {
    if (typeid(T) != object.type()) {
      throw std::runtime_error(std::string("Resource conversion not allowed: ")
                               + object.type().name() + " to " + typeid(T).name());
    }
    return std::any_cast<T>(object);
  }

private:
  std::any object;
};


using ResourceProvider = std::function<Resource()>;
using ResourceConsumer = std::function<void(const Resource&)>;

struct PluginHandle
{
  using ProviderTuple = std::tuple<std::string /*plugin name*/, ResourceProvider>;
  using ConsumerTuple = std::tuple<std::string /*plugin name*/, ResourceConsumer>;
  using Providers = std::list<ProviderTuple>;
  using Consumers = std::list<ConsumerTuple>;
  IPluginDPtr& plugin;
  Providers providers;
  Consumers consumers;
};



namespace cppps {

class Providers
{
public:
  Providers(PluginHandle::Providers& providers, std::string_view source)
    : providers{providers}, source{source} {}

  void add(std::string_view key, const ResourceProvider& provider)
  {
    providers.emplace_back(key, provider);
  }

private:
  PluginHandle::Providers& providers;
  std::string source;
};

class Consumers
{
public:
  Consumers(PluginHandle::Consumers& consumers, std::string_view source)
    : consumers{consumers}, source{source} {}

  void add(std::string_view key, const ResourceConsumer& consumer)
  {
    consumers.emplace_back(key, consumer);
  }

private:
  PluginHandle::Consumers& consumers;
  std::string source;
};

}

class UnresolvedDependencyException: public std::runtime_error {
  using runtime_error::runtime_error;
};

class CircularDependencyException: public std::runtime_error {
  using runtime_error::runtime_error;
};

using PluginDigraph = Digraph<std::string, PluginHandle>;

class IPluginSystem
{
public:
  virtual ~IPluginSystem() = default;

};

using IPluginSystemPtr = std::shared_ptr<IPluginSystem>;

class PluginSystem
{
public:

  using LoadedPlugins = std::list<IPluginDPtr>;

  PluginSystem(const ICliPtr& app,
               const IPluginSystemPtr& dependencyResolver)
    : cli {app}
      , dependencyResolver {dependencyResolver}
  {
  }

  void mergePlugins(LoadedPlugins& plugins)
  {
    this->plugins.merge(plugins);
  }

  void prepare()
  {
    for (auto& plugin: plugins) {
      plugin->prepare(cli);
    }
  }

  void initialize()
  {
    std::map<std::string, Resource> resources;

    PluginDigraph graph([](const auto& handle){
      return handle.plugin->getName();});

    std::map<std::string /*resource key*/, std::string /*source*/> providerOrigins;

    for (auto& plugin: plugins) {
      PluginHandle::Providers handleProviders;
      Providers providers(handleProviders, plugin->getName());
      plugin->submitProviders(providers);

      PluginHandle::Consumers handleConsumers;
      Consumers consumers(handleConsumers, plugin->getName());
      plugin->submitConsumers(consumers);

      for (const auto& provider: handleProviders) {
        providerOrigins.emplace(std::get<0>(provider), plugin->getName());
      }

      graph.addNode(PluginHandle{plugin, handleProviders, handleConsumers});
    }

    // adding edges
    for (auto& plugin: plugins) {
      auto& handle = graph.getNode(plugin->getName());
      for (const auto& [key, consumer]: handle.consumers) {
        auto providerOriginIt = providerOrigins.find(key);
        if (providerOriginIt == providerOrigins.end()) {
          throw UnresolvedDependencyException("Unresolved dependency: resource '"
                                              + key + "' required by plugin '"
                                              + plugin->getName() + "' not found");
        }
        graph.addEdge(plugin->getName(), providerOriginIt->second);
      }
    }

    auto cycles = graph.findCycles();

    if (!cycles.empty()) {
      std::ostringstream message;
      message << "Curcular dependencies have been found: ";
      int n = 1;
      for (const auto& cycle: cycles) {
        message << "(" << n << ") ";
        for (const auto& node: cycle) {
          message << node.plugin->getName() << " -> ";
        }
        message << cycle.front().plugin->getName() << "; ";
      }
      throw CircularDependencyException(message.str());
    }

    // init
    auto sortedPlugins = graph.topologicalSort();
    for (auto& handle: sortedPlugins) {
      auto& plugin = handle.plugin;

      for (auto& [key, consumer]: handle.consumers) {
        consumer(resources.at(key));
      }

      plugin->initialize();

      for (auto& [key, provider]: handle.providers) {
        resources.emplace(key, provider());
      }

      orderedPlugins.emplace_back(std::move(plugin));
    }

  }

  void start()
  {
    for (auto& plugin: orderedPlugins) {
      plugin->start();
    }
  }

  void stop()
  {
    for (auto it = orderedPlugins.rbegin();
         it != orderedPlugins.rend(); ++it) {
      (*it)->stop();
    }
  }

  void unload()
  {
    for (auto it = orderedPlugins.rbegin();
         it != orderedPlugins.rend(); ++it) {
      (*it)->unload();
    }
  }

private:
  ICliPtr cli {nullptr};
  IPluginSystemPtr dependencyResolver {nullptr};
  LoadedPlugins plugins;
  LoadedPlugins orderedPlugins;
};


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
  fakeit::Mock<IPluginSystem> depResolver;

  ICliPtr cliPtr = cli.getFakePtr();
  IPluginSystemPtr depResolverPtr = depResolver.getFakePtr();

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
    When(Method(pluginA, submitProviders)).Do([](Providers& providers) {
      auto providerA = []() {
        return std::make_shared<ProductA>(test::PRODUCT_A_VALUE);
      };
      providers.add(test::PRODUCT_A_KEY, providerA);
    });
  }

  void setupPluginBDependencies()
  {
    Fake(Method(pluginB, submitProviders));
    When(Method(pluginB, submitConsumers)).Do([this](Consumers& consumers) {
      auto consumer = [this](const Resource& res) {
        productAPtr = res.as<ProductAPtr>();
      };
      consumers.add(test::PRODUCT_A_KEY, consumer);
    });
  }

};

} // namespace
} // namespace test

TEST_CASE_METHOD(test::Fixture, "Testing plugins initialization", "[ps_init]")
{
  PluginSystem pluginSystem(cliPtr, depResolverPtr);
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
    When(Method(pluginC, submitConsumers)).Do([](Consumers& consumers) {
      auto consumer = [](const Resource&) {};
      consumers.add(test::PRODUCT_X_KEY, consumer);
    });

    PluginSystem::LoadedPlugins extraPlugins;
    extraPlugins.emplace_back(IPluginDPtr(&pluginC.get(), [](auto*){}));
    pluginSystem.mergePlugins(extraPlugins);

    REQUIRE_THROWS_AS(pluginSystem.initialize(), UnresolvedDependencyException);
  }

  SECTION("When plugin dependencies create a cycle, then an exception is thrown")
  {
    When(Method(pluginA, submitConsumers)).Do([](Consumers& consumers) {
      auto consumer = [](const Resource&) {};
      consumers.add(test::PRODUCT_A_KEY, consumer);
    });

    REQUIRE_THROWS_AS(pluginSystem.initialize(), CircularDependencyException);
  }

}


TEST_CASE_METHOD(test::Fixture, "Testing plugin life cycle", "[ps_cycle]")
{
  PluginSystem pluginSystem(cliPtr, depResolverPtr);
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

