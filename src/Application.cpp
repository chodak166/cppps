// Copyright (c) 2020  Lukasz Chodyla
// Distributed under the Boost Software License v1.0.
// See accompanying file LICENSE.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt for the full license.

#include "cppps/Application.h"
#include "cppps/Cli.h"
#include "cppps/PluginCollector.h"
#include "cppps/BoostPluginLoader.h"

#include <filesystem>
#include <climits>
#include <iostream>

#ifdef __unix
#include <unistd.h>
#endif

using namespace cppps;

Application::Application(const AppInfo& appInfo)
  : appInfo{appInfo}
{
  // empty
}

void Application::setPluginDirectories(const Application::Directories& dirs)
{
  pluginDirs = dirs;
}

std::string Application::getAppDirPath()
{

#ifdef __unix
  char result[PATH_MAX];
  ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
  std::filesystem::path execPath = std::string(result, (count > 0) ? count : 0);
#else
  wchar_t path[MAX_PATH] = { 0 };
  GetModuleFileNameW(NULL, path, MAX_PATH);
  std::filesystem::path execPath = path;
#endif

  return execPath.parent_path().string();
}

void Application::preloadPlugin(IPluginUPtr&& plugin)
{
  IPluginDPtr preloadedPlugin(plugin.get(), [](auto* obj){delete obj;});
  plugin.release();
  preloadedPlugins.push_back(std::move(preloadedPlugin));
}

int Application::exec(int argc, char** argv)
{
  PluginCollector collector;
  collector.addDirectories(pluginDirs);

#ifdef _WIN32
  collector.addPluginExtension("dll");
#else
  collector.addPluginExtension("so");
#endif

  for (auto& preloadedPlugin: preloadedPlugins) {
    pluginSystem.addPlugin(std::move(preloadedPlugin));
  }
  preloadedPlugins.clear();

  auto pluginPaths = collector.collectPlugins();
  BoostPluginLoader loader;
  for (const auto& pluginPath: pluginPaths) {
    auto plugin = loader.load(pluginPath);
    pluginSystem.addPlugin(std::move(plugin));
  }

  auto cli = std::make_shared<Cli>(appInfo);
  pluginSystem.prepare(cli, *this);

  cli->parse(argc, argv);
  if (cli->hasMessage()) {
    std::cout << cli->getMessage();
  }

  if (cli->shouldAppQuit()) {
    return EXIT_SUCCESS;
  }

  pluginSystem.initialize();
  pluginSystem.start();

  if (mainLoop) {
    return mainLoop();
  }

  return EXIT_SUCCESS;
}

int Application::exec()
{
  std::vector<char*> args = {const_cast<char*>("unknown_app_name")};
  return exec(args.size(), args.data());
}

void Application::quit()
{
  pluginSystem.stop();
  pluginSystem.unload();
}

void Application::setMainLoop(const MainLoop& loop)
{
  if (mainLoop) {
    throw std::runtime_error("Possible plugin conflict - main loop already set");
  }
  mainLoop = loop;
}

