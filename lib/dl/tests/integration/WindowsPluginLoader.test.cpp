// Copyright (c) 2020  Lukasz Chodyla
// Distributed under the Boost Software License v1.0.
// See accompanying file LICENSE.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt for the full license.


#include "cppps/dl/exceptions.h"
#include "WindowsPluginLoader.h"
#include "OsUtils.h"

#include <catch2/catch.hpp>

#include <filesystem>

using cppps::WindowsPluginLoader;
using cppps::PluginDeleter;

namespace test {
namespace {

constexpr const char* getPluginExtension();

const auto PLUGIN_DIR = cppps::getProgramDirPath() + "/test_plugins";
const auto PLUGIN_BAD_DIR = cppps::getProgramDirPath() + "/test_plugins_bad";

constexpr auto PLUGIN_A_NAME ="plugin_a";
const auto PLUGIN_A_PATH = PLUGIN_DIR + "/plugin_a." + getPluginExtension();
constexpr auto PLUGIN_B_NAME ="plugin_b";
const auto PLUGIN_B_PATH = PLUGIN_DIR + "/plugin_b." + getPluginExtension();
const auto NON_EXISTENT_PLUGIN_PATH = PLUGIN_DIR + "/plugin_x." + getPluginExtension();
const auto BAD_PLUGIN_PATH = PLUGIN_BAD_DIR + "/plugin_bad." + getPluginExtension();

} // namespace
} // namespace test


TEST_CASE("Testing plugin loader successful library import", "[pl_success]")
{
  WindowsPluginLoader loader;

  SECTION("When a single plugin is loaded, then the returned object is not null")
  {
    auto plugin = loader.load(test::PLUGIN_A_PATH);
    REQUIRE(plugin != nullptr);
  }

//  SECTION("When a single plugin is loaded, then interface methods can be invoked")
//  {
//    auto plugin = loader.load(test::PLUGIN_A_PATH);
//    auto name = plugin->getName();
//    REQUIRE(name == test::PLUGIN_A_NAME);
//  }

//  SECTION("When the loader is destroyed, then all loaded plugins remain valid")
//  {
//    cppps::IPluginDPtr plugin {nullptr, PluginDeleter{nullptr}};

//    { // loader's scope
//      WindowsPluginLoader localLoader;
//      plugin = localLoader.load(test::PLUGIN_A_PATH);
//    }

//    auto name = plugin->getName();
//    REQUIRE(name == test::PLUGIN_A_NAME);
//  }

//  SECTION("When multiple plugins are loaded, then all imported symbols are distinguishable")
//  {
//    auto plugin1 = loader.load(test::PLUGIN_A_PATH);
//    auto plugin2 = loader.load(test::PLUGIN_B_PATH);

//    REQUIRE(test::PLUGIN_A_NAME != test::PLUGIN_B_NAME);
//    REQUIRE(plugin1->getName() == test::PLUGIN_A_NAME);
//    REQUIRE(plugin2->getName() == test::PLUGIN_B_NAME);
//  }

}

//TEST_CASE("Testing plugin loader exceptions", "[pl_exceptions]")
//{
//  WindowsPluginLoader loader;

//  SECTION("When the plugin path is incorrect, then the PluginNotFoundException is thrown")
//  {
//    REQUIRE_THROWS_AS(loader.load(test::NON_EXISTENT_PLUGIN_PATH), cppps::PluginNotFoundException);
//  }

//  SECTION("When the given file is not proper plugin file, then the MakePluginNotFoundException is thrown")
//  {
//    REQUIRE_THROWS_AS(loader.load(test::BAD_PLUGIN_PATH),
//                      cppps::MakePluginNotFoundException);
//  }
//}

namespace test {
namespace {

constexpr const char* getPluginExtension()
{
#ifdef _WIN32
  return "dll";
#else
  return "so";
#endif
}

} // namespace
} // namespace test
