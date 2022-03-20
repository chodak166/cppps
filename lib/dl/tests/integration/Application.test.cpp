// Copyright (c) 2020  Lukasz Chodyla
// Distributed under the MIT License.
// See accompanying file LICENSE.txt for the full license.

#include "cppps/dl/Application.h"
#include "cppps/dl/Cli.h"

#include <iostream>
#include <filesystem>
#include <catch2/catch.hpp>

#include "test_plugins/ITestProduct.h"

using namespace cppps;

namespace test {
namespace {

constexpr auto PARAM_VALUE = "test product value";
const auto PLUGINS_DIR = Application::getAppDirPath() + "/test_plugins";

const AppInfo info {
  "ApplicationTest",
  "Test application",
  "ApplicationTest v1.2.3\n"
};

class SpyPlugin: public IPlugin
{
public:

  enum class State {NEW = 0, LOADED, PREPARED,
                    INITIALIZED, STARTED, STOPPED, UNLOADED};

  struct Values
  {
    ITestProductPtr product;
    std::string param;
    State state {State::NEW};
  };

  SpyPlugin(Values& values)
    : values{values}
  {
    values.state = State::LOADED;
  }

  std::string getName() const override {return "spy_plugin";}
  std::string getVersionString() const override {return "0.0.0";}
  void prepare(const ICliPtr& cli, IApplication& /*app*/) override
  {
    cli->addOption("-p,--param", values.param, "Test product value");
    values.state = State::PREPARED;
  }
  void submitProviders(const SubmitProvider& /*submitProvider*/) override {};
  void submitConsumers(const SubmitConsumer& submitConsumer) override {
    submitConsumer("product_b", [this](const Resource& resource){
      values.product = resource.as<ITestProductPtr>();
    });
  }
  void initialize() override {values.state = State::INITIALIZED;}
  void start() override {values.state = State::STARTED;}
  void stop() override {values.state = State::STOPPED;}
  void unload() override {values.state = State::UNLOADED;}

private:
  Values& values;
};


class SpyPluginWithLoop: public SpyPlugin
{
public:
  using SpyPlugin::SpyPlugin;
  void prepare(const ICliPtr& cli, IApplication& app) override
  {
    SpyPlugin::prepare(cli, app);
    app.setMainLoop([this](){
      ++mainLoopExecutions;
      return 0;
    });

    if (doubleLoop) {
      app.setMainLoop([](){return 0;});
    }
  }

  bool doubleLoop {false};
  int mainLoopExecutions {0};
};


} // namespace
} // namespace test


TEST_CASE("Testing application execution", "[app_exec]")
{
  Application app(test::info);
  app.setPluginDirectories({test::PLUGINS_DIR});

  SECTION("When the application is executed with working plugins, then no exception is thrown")
  {
    REQUIRE_NOTHROW(app.exec());
  }

  SECTION("When a plugin is loaded statically, then it has access to the dynamic resources")
  {
    test::SpyPlugin::Values values;
    app.preloadPlugin(std::make_unique<test::SpyPlugin>(values));
    app.exec();

    CHECK(values.product != nullptr);
  }


  SECTION("When the application main loop is set by plugin, then the main loop is executed once")
  {
    test::SpyPlugin::Values values;
    auto plugin = std::make_unique<test::SpyPluginWithLoop>(values);
    auto& mainLoopExecs = plugin->mainLoopExecutions;
    app.preloadPlugin(std::move(plugin));
    app.exec();
    REQUIRE(mainLoopExecs == 1);
  }


  SECTION("When the application main loop is set twice, then an exception is thrown")
  {
    test::SpyPlugin::Values values;
    auto plugin = std::make_unique<test::SpyPluginWithLoop>(values);
    plugin->doubleLoop = true;
    app.preloadPlugin(std::move(plugin));

    REQUIRE_THROWS(app.exec());
  }


  app.quit();
}

TEST_CASE("Testing application CLI", "[app_cli]")
{
  Application app(test::info);
  app.setPluginDirectories({test::PLUGINS_DIR});

  SECTION("When the help page is requested, then the plugins are not initialized")
  {
    std::vector<char*> args = {const_cast<char*>("app_name"),
                               const_cast<char*>("-h")};

    test::SpyPlugin::Values values;
    app.preloadPlugin(std::make_unique<test::SpyPlugin>(values));
    app.exec(args.size(), args.data());

    CHECK(values.state == test::SpyPlugin::State::PREPARED);
  }

  SECTION("When the version page is requested, then the plugins are not initialized")
  {
    std::vector<char*> args = {const_cast<char*>("app_name"),
                               const_cast<char*>("-v")};

    test::SpyPlugin::Values values;
    app.preloadPlugin(std::make_unique<test::SpyPlugin>(values));
    app.exec(args.size(), args.data());

    CHECK(values.state == test::SpyPlugin::State::PREPARED);
  }

  SECTION("When the application executes, then all arugments can be parsed by the loaded plugins")
  {
    std::vector<char*> args = {const_cast<char*>("app_name"),
                               const_cast<char*>("-p"),
                               const_cast<char*>(test::PARAM_VALUE)};

    test::SpyPlugin::Values values;
    app.preloadPlugin(std::make_unique<test::SpyPlugin>(values));
    app.exec(args.size(), args.data());

    CHECK(values.param == test::PARAM_VALUE);
  }

  app.quit();
}

TEST_CASE("Testing application hooks", "[app_hooks]")
{
  Application app(test::info);
  std::vector<char*> args = {const_cast<char*>("app_name")};
  bool onCli = false;

  app.addOnBeforeCliParseHook([&onCli](const ICliPtr& cli){onCli = true;});
  app.exec(args.size(), args.data());

  CHECK(onCli == true);

  app.quit();
}

TEST_CASE("Testing application destruction", "[app_destruct]")
{
  SECTION("When the application is destroyed without calling the quit method, "
          "then all plugins are unloaded anyway")
  {
    test::SpyPlugin::Values values;

    {
      Application app(test::info);
      app.setPluginDirectories({test::PLUGINS_DIR});
      app.preloadPlugin(std::make_unique<test::SpyPlugin>(values));
      app.exec();
      values.product = nullptr; // don't let the product outlive the app
    }

    REQUIRE(values.state == test::SpyPlugin::State::UNLOADED);
  }
}
