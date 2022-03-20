// Copyright (c) 2020  Lukasz Chodyla
// Distributed under the MIT License.
// See accompanying file LICENSE.txt for the full license.

#ifndef APPLICATION_H
#define APPLICATION_H

#include "cppps/dl/IApplication.h"
#include "cppps/dl/AppInfo.h"
#include "cppps/dl/PluginSystem.h"
#include "cppps/dl/PluginCollector.h"

#include <list>

namespace cppps {

class Application: public IApplication
{
public:
  using Directories = std::list<std::string>;
  using OnBeforeCliParseHook = std::function<void(const ICliPtr& cli)>;

  Application(const AppInfo& appInfo);
  virtual ~Application();

  void setPluginDirectories(const Directories& dirs);
  static std::string getAppDirPath();
  void preloadPlugin(IPluginUPtr&& plugin);
  int exec(int argc, char** argv);
  int exec();

  void addOnBeforeCliParseHook(const OnBeforeCliParseHook& hook);

  // IApplication
  void quit() override;
  void setMainLoop(const MainLoop& loop) override;

private:
  AppInfo appInfo;
  Directories pluginDirs;
  PluginSystem pluginSystem;
  PluginSystem::LoadedPlugins preloadedPlugins;
  MainLoop mainLoop {nullptr};
  bool quitCalled {false};
  std::list<OnBeforeCliParseHook> onBeforeCliParseHooks;

private:
  enum class CliParseResult {QUIT, CONTINUE};
  PluginCollector::Paths collectPlugins();
  void loadPlugins(const PluginCollector::Paths& pluginPaths);
  CliParseResult parseCli(int argc, char** argv);
  int execMainLoop();
  void setupInterruptHandler();

};


} // namespace cppps

#endif // APPLICATION_H
