// Copyright (c) 2020  Lukasz Chodyla
// Distributed under the Boost Software License v1.0.
// See accompanying file LICENSE.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt for the full license.


#include "cppps/IPlugin.h"
#include "cppps/exceptions.h"

#include <catch2/catch.hpp>
//#include <fakeit/catch/fakeit.hpp>

#include <filesystem>
#include <fstream>
#include <limits>

#include <nixlab/stdeasylog>

//NOTE: use -Wno-disabled-macro-expansion to suppress fakeit macro warnings
//using namespace fakeit;

namespace {

}


namespace test {
namespace {

const auto TMP_DIR = std::filesystem::temp_directory_path().string() + "/cppps_test";
const auto PLUGIN_DIR_A = TMP_DIR + "/cppps_test_plugins_1";
const auto PLUGIN_DIR_B = TMP_DIR + "/cppps_test_plugins_2";
const auto PLUGIN_DIR_C = TMP_DIR + "/cppps_test_plugins_3";

const auto PLUGIN_A_1_FILE          = PLUGIN_DIR_A + "/libplugin_a1.so";
const auto PLUGIN_A_1_FILE_V        = PLUGIN_DIR_A + "/libplugin_a1.so.1";
const auto PLUGIN_A_1_FILE_VV       = PLUGIN_DIR_A + "/libplugin_a1.so.1.1";
const auto PLUGIN_A_1_FILE_VVV      = PLUGIN_DIR_A + "/libplugin_a1.so.1.1.1";
const auto PLUGIN_A_1_FILE_VVV_NEW  = PLUGIN_DIR_A + "/libplugin_a1.so.1.1.2";

const auto PLUGIN_A_2_FILE          = PLUGIN_DIR_A + "/libplugin_a2.so";

const auto PLUGIN_B_1_FILE          = PLUGIN_DIR_B + "/libplugin_b1.so";
const auto PLUGIN_B_2_FILE          = PLUGIN_DIR_B + "/libplugin_b2.so";
const auto PLUGIN_B_3_FILE          = PLUGIN_DIR_B + "/libplugin_b2.plugin";
const auto OTHER_B_4_FILE           = PLUGIN_DIR_B + "/libplugin_b2.txt";

const auto PLUGIN_C_1_FILE          = PLUGIN_DIR_C + "/libplugin_c1.so";
const auto PLUGIN_C_2_FILE          = PLUGIN_DIR_C + "/libplugin_c2.so";

void createTestFiles();


} // namespace
} // namespace test


TEST_CASE("Testing plugin collector", "[plugin_collector]")
{
  test::createTestFiles();
  PluginCollector collector;
  collector.addPluginExtension("so");

  SECTION("When no directories were added, then no plugins are collected")
  {
    //TODO
  }

  SECTION("When a single directory was added, then all files within that directory ending with given extension are collected")
  {
    //TODO
  }

  SECTION("When multiple directories were added, then all files within that directories ending with given extension are collected")
  {
    //TODO
  }

  SECTION("When a matched file name has version suffix, then it's also collected")
  {
    //TODO
  }

  SECTION("When an extension of a file within added directories is not matched, then it's not collected")
  {
    //TODO
  }

  SECTION("When extra directory environment variable was set, then matched files from given directory are collected")
  {

  }

  SECTION("When extra directory environment variable contains multiple directories, then matched files from given directories are collected")
  {

  }

  SECTION("When extra plugin environment variable was set, then given path is collected unconditionally")
  {

  }

  SECTION("When extra plugin environment variable contains multiple paths, then given pathss are collected unconditionally")
  {

  }

  SECTION("When no plugin extension was added, then no plugins are collected")
  {
    //TODO
  }

  SECTION("When multiple directories and plugins were added, then all matched and extra files are collected")
  {
    //TODO
  }

}


namespace test {
namespace {

void writeRandomFile(std::string_view filename, int bytes = 1024)
{
  std::ofstream file(filename.data(), std::ofstream::binary);
  if (file.is_open()) {
    for (int i = 0; i < bytes; ++i) {
      file << static_cast<char>(rand() % std::numeric_limits<char>::max());
    }
    file.close();
  }
  else {
    throw std::runtime_error("Cannot open file: " + std::string(filename));
  }
}

void createTestFiles()
{
  LOG(DEBUG) << "Creating files in: " << TMP_DIR;
  std::filesystem::create_directories(PLUGIN_DIR_A);
  std::filesystem::create_directories(PLUGIN_DIR_B);
  std::filesystem::create_directories(PLUGIN_DIR_C);

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
}

} // namespace
} // namespace test

