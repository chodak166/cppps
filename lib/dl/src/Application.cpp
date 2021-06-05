// Copyright (c) 2020  Lukasz Chodyla
// Distributed under the Boost Software License v1.0.
// See accompanying file LICENSE.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt for the full license.

#include "cppps/dl/Application.h"
#include "cppps/dl/Cli.h"
#include "PluginLoader.h"

#include "OsUtils.h"

#include <filesystem>
#include <iostream>
#include <csignal>
#include <functional>

using namespace cppps;

namespace  {

static bool instanceExists {false};
std::function<void(int)> quitApp {[](int){}};

void signalHandler(int signum) {
    quitApp(signum);
}

}

Application::Application(const AppInfo& appInfo)
  : appInfo{appInfo}
{
  if (instanceExists) {
    throw std::runtime_error("Threre can be only one instance of the Application class");
  }
  instanceExists = true;
}

Application::~Application()
{
  if (!quitCalled) {
    quit();
  }
  pluginSystem.unload();
  instanceExists = false;
}

void Application::setPluginDirectories(const Application::Directories& dirs)
{
  pluginDirs = dirs;
}

std::string Application::getAppDirPath()
{
  return cppps::getProgramDirPath();
}

void Application::preloadPlugin(IPluginUPtr&& plugin)
{
  IPluginDPtr preloadedPlugin(plugin.get(), [](auto* obj){delete obj;});
  plugin.release();
  preloadedPlugins.push_back(std::move(preloadedPlugin));
}

int Application::exec(int argc, char** argv)
{
  if (appInfo.interruptable) {
    setupInterruptHandler();
  }

  auto pluginPaths = collectPlugins();
  loadPlugins(pluginPaths);

  auto parseResult = parseCli(argc, argv);
  if (parseResult == CliParseResult::QUIT) {
    return EXIT_SUCCESS;
  }

  pluginSystem.initialize();
  pluginSystem.start();

  return execMainLoop();
}

int Application::exec()
{
  std::vector<char*> args = {const_cast<char*>("unknown_app_name")};
  return exec(args.size(), args.data());
}

void Application::addOnBeforeCliParseHook(const OnBeforeCliParseHook& hook)
{
  onBeforeCliParseHooks.push_back(hook);
}

void Application::quit()
{
  quitCalled = true;
  pluginSystem.stop();
}

void Application::setMainLoop(const MainLoop& loop)
{
  if (mainLoop) {
    throw std::runtime_error("Possible plugin conflict - main loop already set");
  }
  mainLoop = loop;
}

PluginCollector::Paths Application::collectPlugins()
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

  return collector.collectPlugins();
}

void Application::loadPlugins(const PluginCollector::Paths& pluginPaths)
{
  auto loader = cppps::getPluginLoader();
  for (const auto& pluginPath: pluginPaths) {
    auto plugin = loader.load(pluginPath);
    pluginSystem.addPlugin(std::move(plugin));
  }
}

Application::CliParseResult Application::parseCli(int argc, char** argv)
{
  auto cli = std::make_shared<Cli>(appInfo);

  pluginSystem.prepare(cli, *this);

  for(auto& hook: onBeforeCliParseHooks) {
    hook(cli);
  }

  cli->parse(argc, argv);
  if (cli->hasMessage()) {
    std::cout << cli->getMessage();
  }

  if (cli->shouldAppQuit()) {
    return CliParseResult::QUIT;
  }

  return CliParseResult::CONTINUE;
}

int Application::execMainLoop()
{
  int result = EXIT_SUCCESS;
  if (mainLoop) {
    result = mainLoop();
    // dispose main loop handle in case
    // a plugin passed lambda capturing "this":
    mainLoop = nullptr;
  }
  return result;
}

void Application::setupInterruptHandler()
{
  quitApp = [this](int){
    quit();
  };

  signal(SIGINT, signalHandler);
  signal(SIGTERM, signalHandler);
}
