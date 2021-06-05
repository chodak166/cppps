// Copyright (c) 2020  Lukasz Chodyla
// Distributed under the Boost Software License v1.0.
// See accompanying file LICENSE.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt for the full license.


#include "cppps/dl/PluginCollector.h"
#include "cppps/dl/IPlugin.h"
#include "cppps/dl/exceptions.h"

#define CATCH_CONFIG_EXTERNAL_INTERFACES
#include <catch2/catch.hpp>

#include <filesystem>
#include <fstream>
#include <limits>

#include <list>
#include <regex>

using cppps::PluginCollector;
using Path = std::filesystem::path;

namespace test {
namespace {

const auto TMP_DIR = std::filesystem::temp_directory_path().string() + "/cppps_test";
const auto PLUGIN_DIR_A = TMP_DIR + "/cppps_test_plugins_1";
const auto PLUGIN_DIR_B = TMP_DIR + "/cppps_test_plugins_2";
const auto PLUGIN_DIR_C = TMP_DIR + "/cppps_test_plugins_3";
const auto PLUGIN_DIR_D = TMP_DIR + "/cppps_test_plugins_4";

const Path PLUGIN_A_1_FILE          = PLUGIN_DIR_A + "/libplugin_a1.so";
const Path PLUGIN_A_1_FILE_V        = PLUGIN_DIR_A + "/libplugin_a1.so.1";
const Path PLUGIN_A_1_FILE_VV       = PLUGIN_DIR_A + "/libplugin_a1.so.1.1";
const Path PLUGIN_A_1_FILE_VVV      = PLUGIN_DIR_A + "/libplugin_a1.so.1.1.1";
const Path PLUGIN_A_1_FILE_VVV_NEW  = PLUGIN_DIR_A + "/libplugin_a1.so.1.2";
const Path PLUGIN_A_2_FILE          = PLUGIN_DIR_A + "/libplugin_a2.so";

const Path PLUGIN_B_1_FILE          = PLUGIN_DIR_B + "/libplugin_b1.so";
const Path PLUGIN_B_2_FILE          = PLUGIN_DIR_B + "/libplugin_b2.so";
const Path PLUGIN_B_3_FILE          = PLUGIN_DIR_B + "/libplugin_b2.plugin";
const Path OTHER_B_4_FILE           = PLUGIN_DIR_B + "/libplugin_b2.txt";

const Path PLUGIN_C_1_FILE          = PLUGIN_DIR_C + "/libplugin_c1.so";
const Path PLUGIN_C_2_FILE          = PLUGIN_DIR_C + "/libplugin_c2.so";

const Path PLUGIN_D_1_FILE          = PLUGIN_DIR_D + "/libplugin_d1.so.1.2.3";

constexpr auto PLUGIN_DIR_A_COUNT = 2;
constexpr auto PLUGIN_DIR_B_COUNT = 2;
constexpr auto PLUGIN_DIR_C_COUNT = 2;
constexpr auto PLUGIN_DIR_D_COUNT = 1;

const auto EXTRA_FILE_ENV_NAME = "PS_TEST_PLUGIN";
const auto EXTRA_PATH_ENV_NAME = "PS_TEST_PLUGIN_PATH";

void createTestFiles();
void setEnvVariable(const std::string& var, const std::string& value);


} // namespace
} // namespace test

// use test event listener to create test files only once
struct TestEventListener : Catch::TestEventListenerBase {
    using TestEventListenerBase::TestEventListenerBase;
    void testCaseStarting( Catch::TestCaseInfo const& /*testInfo*/ ) override {
        test::createTestFiles();
    }
};
CATCH_REGISTER_LISTENER( TestEventListener )


TEST_CASE("Testing plugin collector", "[plugin_collector]")
{
  PluginCollector collector;
  collector.addPluginExtension("so");

  SECTION("When no directories were added, then no plugins are collected")
  {
    auto files = collector.collectPlugins();
    REQUIRE(files.empty());
  }

  SECTION("When an extension of a file within added directories is not matched, then it's not collected")
  {
    collector.addDirectory(test::PLUGIN_DIR_B);
    auto files = collector.collectPlugins();

    auto it = std::find(files.begin(), files.end(), test::OTHER_B_4_FILE);
    REQUIRE(it == files.end());
  }

  SECTION("When a single directory was added, then all files within that directory ending with given extension are collected")
  {
    collector.addDirectory(test::PLUGIN_DIR_C);
    auto files = collector.collectPlugins();
    files.sort();

    auto it = files.begin();
    REQUIRE(files.size() == test::PLUGIN_DIR_C_COUNT);
    REQUIRE(*it == test::PLUGIN_C_1_FILE); std::advance(it, 1);
    REQUIRE(*it == test::PLUGIN_C_2_FILE); std::advance(it, 1);
    REQUIRE(it == files.end());
  }

  SECTION("When multiple directories were added, then all files within that directories ending with given extension are collected")
  {
    collector.addDirectories({test::PLUGIN_DIR_B, test::PLUGIN_DIR_C});
    auto files = collector.collectPlugins();
    files.sort();

    auto it = files.begin();
    REQUIRE(files.size() == test::PLUGIN_DIR_B_COUNT + test::PLUGIN_DIR_C_COUNT);
    REQUIRE(*it == test::PLUGIN_B_1_FILE); std::advance(it, 1);
    REQUIRE(*it == test::PLUGIN_B_2_FILE); std::advance(it, 1);
    REQUIRE(*it == test::PLUGIN_C_1_FILE); std::advance(it, 1);
    REQUIRE(*it == test::PLUGIN_C_2_FILE); std::advance(it, 1);
    REQUIRE(it == files.end());
  }

  SECTION("When a matched file name has version suffix, then it's also collected")
  {
    collector.addDirectory(test::PLUGIN_DIR_D);
    auto files = collector.collectPlugins();
    files.sort();

    auto it = files.begin();
    REQUIRE(files.size() == test::PLUGIN_DIR_D_COUNT);
    REQUIRE(*it == test::PLUGIN_D_1_FILE);
  }

  SECTION("When there are multiple files with the same name but different versions, then the one with the highest version is collected")
  {
    collector.addDirectory(test::PLUGIN_DIR_A);
    auto files = collector.collectPlugins();
    files.sort();

    auto it = files.begin();
    REQUIRE(files.size() == test::PLUGIN_DIR_A_COUNT);
    REQUIRE(*it == test::PLUGIN_A_1_FILE_VVV_NEW); std::advance(it, 1);
    REQUIRE(*it == test::PLUGIN_A_2_FILE); std::advance(it, 1);
    REQUIRE(it == files.end());
  }

  SECTION("When extra directory environment variable is empty, then no exception is thrown")
  {
    collector.enablePathEnvVariable(test::EXTRA_PATH_ENV_NAME);
    REQUIRE_NOTHROW(collector.collectPlugins());
  }


  SECTION("When extra directory environment variable was set, then matched files from given directory are collected")
  {
    collector.enablePathEnvVariable(test::EXTRA_PATH_ENV_NAME);
    test::setEnvVariable(test::EXTRA_PATH_ENV_NAME, test::PLUGIN_DIR_C);

    auto files = collector.collectPlugins();
    files.sort();

    auto it = files.begin();
    REQUIRE(files.size() == test::PLUGIN_DIR_C_COUNT);
    REQUIRE(*it == test::PLUGIN_C_1_FILE); std::advance(it, 1);
    REQUIRE(*it == test::PLUGIN_C_2_FILE); std::advance(it, 1);
    REQUIRE(it == files.end());
  }

  SECTION("When extra directory environment variable contains multiple directories, then matched files from given directories are collected")
  {
    collector.enablePathEnvVariable(test::EXTRA_PATH_ENV_NAME);
    test::setEnvVariable(test::EXTRA_PATH_ENV_NAME, test::PLUGIN_DIR_C + ';' + test::PLUGIN_DIR_D);

    auto files = collector.collectPlugins();
    files.sort();

    auto it = files.begin();
    REQUIRE(files.size() == test::PLUGIN_DIR_C_COUNT + test::PLUGIN_DIR_D_COUNT);
    REQUIRE(*it == test::PLUGIN_C_1_FILE); std::advance(it, 1);
    REQUIRE(*it == test::PLUGIN_C_2_FILE); std::advance(it, 1);
    REQUIRE(*it == test::PLUGIN_D_1_FILE); std::advance(it, 1);
    REQUIRE(it == files.end());
  }

  SECTION("When extra plugin environment variable was set, then given path is collected unconditionally")
  {
    collector.enableFileEnvVariable(test::EXTRA_FILE_ENV_NAME);
    test::setEnvVariable(test::EXTRA_FILE_ENV_NAME, test::PLUGIN_B_3_FILE.string());

    auto files = collector.collectPlugins();
    files.sort();

    auto it = files.begin();
    REQUIRE(files.size() == 1);
    REQUIRE(*it == test::PLUGIN_B_3_FILE);
  }

  SECTION("When extra plugin environment variable contains multiple paths, then given paths are collected unconditionally")
  {
    collector.enableFileEnvVariable(test::EXTRA_FILE_ENV_NAME);
    test::setEnvVariable(test::EXTRA_FILE_ENV_NAME, test::PLUGIN_A_1_FILE_VV.string()
                         + ';' + test::PLUGIN_B_3_FILE.string());

    auto files = collector.collectPlugins();
    files.sort();

    auto it = files.begin();
    REQUIRE(files.size() == 2);
    REQUIRE(*it == test::PLUGIN_A_1_FILE_VV); std::advance(it, 1);
    REQUIRE(*it == test::PLUGIN_B_3_FILE); std::advance(it, 1);
    REQUIRE(it == files.end());
  }

  SECTION("When no plugin extension was added, then no plugins are collected")
  {
    PluginCollector localCollector;
    localCollector.addDirectory(test::PLUGIN_DIR_C);
    auto files = localCollector.collectPlugins();
    REQUIRE(files.empty());
  }

  // that might be redundant
  SECTION("When multiple directories and plugins were added, then all matched and extra files are collected")
  {
    collector.addDirectory(test::PLUGIN_DIR_A);

    collector.enableFileEnvVariable(test::EXTRA_FILE_ENV_NAME);
    test::setEnvVariable(test::EXTRA_FILE_ENV_NAME, test::OTHER_B_4_FILE.string());

    collector.enablePathEnvVariable(test::EXTRA_PATH_ENV_NAME);
    test::setEnvVariable(test::EXTRA_PATH_ENV_NAME, test::PLUGIN_DIR_C);

    auto files = collector.collectPlugins();
    files.sort();

    auto it = files.begin();
    REQUIRE(files.size() == test::PLUGIN_DIR_A_COUNT + test::PLUGIN_DIR_C_COUNT + 1);
    REQUIRE(*it == test::PLUGIN_A_1_FILE_VVV_NEW); std::advance(it, 1);
    REQUIRE(*it == test::PLUGIN_A_2_FILE); std::advance(it, 1);
    REQUIRE(*it == test::OTHER_B_4_FILE); std::advance(it, 1);
    REQUIRE(*it == test::PLUGIN_C_1_FILE); std::advance(it, 1);
    REQUIRE(*it == test::PLUGIN_C_2_FILE); std::advance(it, 1);
    REQUIRE(it == files.end());
  }

}


namespace test {
namespace {

void writeRandomFile(const Path& path, int bytes = 1024)
{
  std::ofstream file(path.string(), std::ofstream::binary);
  if (file.is_open()) {
    for (int i = 0; i < bytes; ++i) {
      file << static_cast<char>(rand() % std::numeric_limits<char>::max());
    }
    file.close();
  }
  else {
    throw std::runtime_error("Cannot open file: " + path.string());
  }
}

void setEnvVariable(const std::string& var, const std::string& value)
{
#ifdef __unix
  setenv(var.c_str(), value.c_str(), 1);
#else
  auto str = var + "=" + value;
  auto envExpression = _strdup(str.c_str());
  _putenv(envExpression);
#endif
}

void createTestFiles()
{
  std::filesystem::create_directories(PLUGIN_DIR_A);
  std::filesystem::create_directories(PLUGIN_DIR_B);
  std::filesystem::create_directories(PLUGIN_DIR_C);
  std::filesystem::create_directories(PLUGIN_DIR_D);

  writeRandomFile( PLUGIN_A_1_FILE         );
  writeRandomFile( PLUGIN_A_1_FILE_V       );
  writeRandomFile( PLUGIN_A_1_FILE_VV      );
  writeRandomFile( PLUGIN_A_1_FILE_VVV     );
  writeRandomFile( PLUGIN_A_1_FILE_VVV_NEW );
  writeRandomFile( PLUGIN_A_2_FILE         );
  writeRandomFile( PLUGIN_B_1_FILE         );
  writeRandomFile( PLUGIN_B_2_FILE         );
  writeRandomFile( PLUGIN_B_3_FILE         );
  writeRandomFile( OTHER_B_4_FILE          );
  writeRandomFile( PLUGIN_C_1_FILE         );
  writeRandomFile( PLUGIN_C_2_FILE         );
  writeRandomFile( PLUGIN_D_1_FILE         );
}

} // namespace
} // namespace test

