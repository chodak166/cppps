// Copyright (c) 2020  Lukasz Chodyla
// Distributed under the Boost Software License v1.0.
// See accompanying file LICENSE.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt for the full license.

#ifndef PLUGINSYSTEM_H
#define PLUGINSYSTEM_H

#include "cppps/IPlugin.h"
#include <list>

namespace cppps {

/**
 * @brief Plugin life-cycle controller class.
 *
 * The PluginSystem instance will take ownership of given
 * IPlugin objects and go throug all their life-cycle stages.
 *
 * It is guaranteed that:
 * a) the IPlugin::initialize() method will be called after every
 * consumer of the plugin gets his resources provided by other plugins;
 * b) every provider will be called after its plugin successfull
 * initialization;
 * c) an exception is thrown when there is at least one
 * unresolved dependency (i.e. consumer with no matched provider);
 * d) an exception is thrown when there is at leas one
 * circular dependency.
 *
 */
class PluginSystem
{
public:
  using LoadedPlugins = std::list<IPluginDPtr>;

  PluginSystem();
  void addPlugin(IPluginDPtr&& plugin);
  void mergePlugins(LoadedPlugins& plugins);
  void prepare(const ICliPtr& cli);
  void initialize();
  void start();
  void stop();
  void unload();

private:
  LoadedPlugins uninitializedPlugins;
  LoadedPlugins initializedPlugins;
};

} // namespace cppps

#endif // PLUGINSYSTEM_H
