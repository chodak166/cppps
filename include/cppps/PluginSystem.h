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
 * It is guaranteed that for plugin P:
 * a) every P::initialize() method will be called after every
 * consumer of P gets his resources provided by other plugins;
 * b) every provider of P will be called after successfull
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

  PluginSystem(const ICliPtr& app);
  void mergePlugins(LoadedPlugins& plugins);
  void prepare();
  void initialize();
  void start();
  void stop();
  void unload();

private:
  ICliPtr cli {nullptr};
  LoadedPlugins uninitializedPlugins;
  LoadedPlugins initializedPlugins;
};

} // namespace cppps

#endif // PLUGINSYSTEM_H
