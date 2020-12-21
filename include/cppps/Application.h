// Copyright (c) 2020  Lukasz Chodyla
// Distributed under the Boost Software License v1.0.
// See accompanying file LICENSE.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt for the full license.

#ifndef APPLICATION_H
#define APPLICATION_H

#include "cppps/AppInfo.h"
#include "cppps/PluginSystem.h"

namespace cppps {

class Application
{
public:
  using Directories = std::list<std::string>;

  Application(const AppInfo& appInfo);
  virtual ~Application() = default;

  void setPluginDirectories(const Directories& dirs);
  static std::string getAppDirPath();
  void preloadPlugin(IPluginUPtr&& plugin);
  int exec(int argc, char** argv);
  int exec();
  void quit();

private:
  AppInfo appInfo;
  Directories pluginDirs;
  PluginSystem pluginSystem;
  PluginSystem::LoadedPlugins preloadedPlugins;
};


} // namespace cppps

#endif // APPLICATION_H
