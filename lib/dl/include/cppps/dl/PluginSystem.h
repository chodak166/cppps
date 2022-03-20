// Copyright (c) 2020  Lukasz Chodyla
// Distributed under the MIT License.
// See accompanying file LICENSE.txt for the full license.

#ifndef PLUGINSYSTEM_H
#define PLUGINSYSTEM_H

#include "cppps/dl/IPlugin.h"
#include <list>

namespace cppps {

class IApplication;

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
  void prepare(const ICliPtr& cli, IApplication& app);
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
