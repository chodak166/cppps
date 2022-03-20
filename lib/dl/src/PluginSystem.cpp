// Copyright (c) 2020  Lukasz Chodyla
// Distributed under the MIT License.
// See accompanying file LICENSE.txt for the full license.

#include "cppps/dl/PluginSystem.h"
#include "cppps/dl/Digraph.h"
#include "cppps/dl/exceptions.h"

#include <map>
#include <any>
#include <sstream>

using namespace cppps;

namespace {

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

using PluginDigraph = Digraph<std::string, PluginHandle>;

class PluginInitializer
{
public:
  void initializePlugins(PluginSystem::LoadedPlugins& uninitializedPlugins,
                         PluginSystem::LoadedPlugins& initializedPlugins);

private:
  std::map<std::string, Resource> resources;
  std::map<std::string /*resource key*/,
           std::string /*source*/> providerOrigins;

  PluginDigraph graph {
    ([](const auto& pluginHandle) {
      return pluginHandle.plugin->getName();
    })
  };

private:
  void addGraphNodes(PluginSystem::LoadedPlugins& plugins);
  void addGraphEdges(PluginSystem::LoadedPlugins& plugins);
  void assertNoCycles();
  void initializePlugins(PluginSystem::LoadedPlugins& orderedPlugins);

};

} // namespace


PluginSystem::PluginSystem()
{
  // empty
}

void PluginSystem::addPlugin(IPluginDPtr&& plugin)
{
  uninitializedPlugins.push_back(std::move(plugin));
}

void PluginSystem::mergePlugins(LoadedPlugins& plugins)
{
  this->uninitializedPlugins.merge(plugins);
}

void PluginSystem::prepare(const ICliPtr& cli, IApplication& app)
{
  for (auto& plugin: uninitializedPlugins) {
    plugin->prepare(cli, app);
  }
}

void PluginSystem::initialize()
{
  PluginInitializer initializer;
  initializer.initializePlugins(uninitializedPlugins, initializedPlugins);
  uninitializedPlugins.clear();
}

void PluginSystem::start()
{
  for (auto& plugin: initializedPlugins) {
    plugin->start();
  }
}

void PluginSystem::stop()
{
  for (auto it = initializedPlugins.rbegin();
       it != initializedPlugins.rend(); ++it) {
    (*it)->stop();
  }
}

void PluginSystem::unload()
{
  for (auto it = initializedPlugins.rbegin();
       it != initializedPlugins.rend(); ++it) {
    (*it)->unload();
    (*it) = nullptr; // preserve destroying order
  }
  initializedPlugins.clear();
}

// -------------------

namespace {

void PluginInitializer::initializePlugins(PluginSystem::LoadedPlugins& uninitializedPlugins,
                       PluginSystem::LoadedPlugins& initializedPlugins)
{
  addGraphNodes(uninitializedPlugins);
  addGraphEdges(uninitializedPlugins);
  assertNoCycles();
  initializePlugins(initializedPlugins);
}

void PluginInitializer::addGraphNodes(PluginSystem::LoadedPlugins& plugins)
{
  for (auto& plugin: plugins) {
    PluginHandle::Providers handleProviders;

    SubmitProvider submitProvider = [&handleProviders](auto key, auto& provider) {
      handleProviders.emplace_back(key, provider);};

    plugin->submitProviders(submitProvider);

    PluginHandle::Consumers handleConsumers;
    SubmitConsumer submitConsumer = [&handleConsumers](auto key, const auto& consumer) {
      handleConsumers.emplace_back(key, consumer);};

    plugin->submitConsumers(submitConsumer);

    for (const auto& provider: handleProviders) {
      providerOrigins.emplace(std::get<0>(provider), plugin->getName());
    }
    graph.addNode(PluginHandle{plugin, handleProviders, handleConsumers});
  }
}

void PluginInitializer::addGraphEdges(PluginSystem::LoadedPlugins& plugins)
{
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
}

void PluginInitializer::assertNoCycles()
{
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
}

void PluginInitializer::initializePlugins(PluginSystem::LoadedPlugins& orderedPlugins)
{
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

} // namespace
