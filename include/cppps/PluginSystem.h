// Copyright (c) 2020  Lukasz Chodyla
// Distributed under the Boost Software License v1.0.
// See accompanying file LICENSE.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt for the full license.

#ifndef PLUGINSYSTEM_H
#define PLUGINSYSTEM_H

#include "cppps/IPlugin.h"
#include <list>

namespace cppps {

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
