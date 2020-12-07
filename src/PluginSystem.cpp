// Copyright (c) 2020  Lukasz Chodyla
// Distributed under the Boost Software License v1.0.
// See accompanying file LICENSE.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt for the full license.

#include "cppps/PluginSystem.h"
#include "cppps/Digraph.h"
#include "cppps/IPlugin.h"
#include "cppps/Resource.h"
#include "cppps/exceptions.h"

#include <map>
#include <any>
#include <sstream>

using cppps::PluginSystem;
using cppps::Digraph;
using cppps::Resource;
using cppps::IPluginDPtr;
using cppps::ResourceProvider;
using cppps::ResourceConsumer;

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

} // namespace


PluginSystem::PluginSystem(const ICliPtr& app)
  : cli {app}
{
}

void PluginSystem::mergePlugins(LoadedPlugins& plugins)
{
  this->plugins.merge(plugins);
}

void PluginSystem::prepare()
{
  for (auto& plugin: plugins) {
    plugin->prepare(cli);
  }
}

void PluginSystem::initialize()
{
  std::map<std::string, Resource> resources;

  PluginDigraph graph([](const auto& handle){
    return handle.plugin->getName();});

  std::map<std::string /*resource key*/, std::string /*source*/> providerOrigins;

  for (auto& plugin: plugins) {
    PluginHandle::Providers handleProviders;

    SubmitProvider submitProvider = [&handleProviders](auto key, auto& provider) {
      handleProviders.emplace_back(key, provider);
    };

    //Providers providers(handleProviders, plugin->getName());
    plugin->submitProviders(submitProvider);

    PluginHandle::Consumers handleConsumers;
   // Consumers consumers(handleConsumers, plugin->getName());
    SubmitConsumer submitConsumer = [&handleConsumers](auto key, const auto& consumer) {
      handleConsumers.emplace_back(key, consumer);
    };
    plugin->submitConsumers(submitConsumer);

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

void PluginSystem::start()
{
  for (auto& plugin: orderedPlugins) {
    plugin->start();
  }
}

void PluginSystem::stop()
{
  for (auto it = orderedPlugins.rbegin();
       it != orderedPlugins.rend(); ++it) {
    (*it)->stop();
  }
}

void PluginSystem::unload()
{
  for (auto it = orderedPlugins.rbegin();
       it != orderedPlugins.rend(); ++it) {
    (*it)->unload();
  }
}
