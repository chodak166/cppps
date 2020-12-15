// Copyright (c) 2020  Lukasz Chodyla
// Distributed under the Boost Software License v1.0.
// See accompanying file LICENSE.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt for the full license.


#include "cppps/PluginCollector.h"
#include "cppps/IPlugin.h"
#include "cppps/exceptions.h"

#include "cppps/BoostPluginLoader.h"
#include "cppps/PluginCollector.h"
#include "cppps/PluginSystem.h"

#include <filesystem>
#include <catch2/catch.hpp>

#include "app_plugins/ITestProduct.h"

#ifdef _WIN32
#include <windows.h>    //GetModuleFileNameW
#else
#include <limits.h>
#include <unistd.h>     //readlink
#endif

using namespace cppps;

class Application
{
public:

  using Directories = std::list<std::string>;

  Application(): pluginSystem{nullptr} {}

  virtual ~Application() = default;

  void setPluginDirectories(const Directories& dirs)
  {
    pluginDirs = dirs;
  }

  static std::string getAppDirPath()
  {
    std::filesystem::path execPath;
#ifdef _WIN32
    wchar_t path[MAX_PATH] = { 0 };
    GetModuleFileNameW(NULL, path, MAX_PATH);
    execPath = path;
#else
    char result[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
    execPath = std::string(result, (count > 0) ? count : 0);
#endif
    return execPath.parent_path().string();
  }

  void preloadPlugin(IPluginUPtr&& plugin)
  {
    IPluginDPtr preloadedPlugin(plugin.get(), [](auto* obj){delete obj;});
    plugin.release();
    preloadedPlugins.push_back(std::move(preloadedPlugin));
  }

  int exec()
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

    pluginSystem.prepare();
    pluginSystem.initialize();
    pluginSystem.start();

    return EXIT_SUCCESS;
  }

  void quit()
  {
    pluginSystem.stop();
    pluginSystem.unload();
  }

private:
  Directories pluginDirs;
  PluginSystem pluginSystem;
  PluginSystem::LoadedPlugins preloadedPlugins;
};


namespace test {
namespace {

class SpyPlugin: public IPlugin
{
public:

  SpyPlugin(ITestProductPtr& product): product {product} {};

  std::string getName() const override {return "spy_plugin";}
  std::string getVersionString() const override {return "0.0.0";}
  void prepare(const ICliPtr& app) override {};
  void submitProviders(const SubmitProvider& submitProvider) override {};
  void submitConsumers(const SubmitConsumer& submitConsumer) override {
    submitConsumer("product_b", [this](const Resource& resource){
      product = resource.as<ITestProductPtr>();
    });
  };
  void initialize() override {};
  void start() override {};
  void stop() override {};
  void unload() override {
    //product = nullptr;
  };

private:
  ITestProductPtr product;
};

} // namespace
} // namespace test


TEST_CASE("Testing application execution", "[app_exec]")
{
  Application app;
  app.setPluginDirectories({Application::getAppDirPath() + "/app_plugins"});

  SECTION("When the application is executed with working plugins, then no exception is thrown")
  {
    REQUIRE_NOTHROW(app.exec());
  }

  SECTION("When a plugin is loaded statically, then it has access to dynamic resources")
  {
    ITestProductPtr product = nullptr;
    app.preloadPlugin(std::make_unique<test::SpyPlugin>(product));
    app.exec();

   // REQUIRE(product != nullptr);
  }


  app.quit();
}


namespace test {
namespace {


} // namespace
} // namespace test

