// Copyright (c) 2020  Lukasz Chodyla
// Distributed under the Boost Software License v1.0.
// See accompanying file LICENSE.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt for the full license.

#include "cppps/dl/PluginCollector.h"

#include <map>
#include <vector>
#include <algorithm>
#include <regex>
#include <filesystem>

using cppps::PluginCollector;

namespace {

constexpr auto FULL_VERSION_MATCH_SIZE = 3;
constexpr auto NAME_MATCH_INDEX = 1;
constexpr auto VERSION_MATCH_INDEX = 2;

struct PathEntry
{
  bool valid {false};
  std::filesystem::path path;
  std::string name;
  std::string version;
};

using PathMap = std::map<std::string /*name*/, PathEntry>;

void appendDirectoriesScanResults(PluginCollector::Directories& dirs,
                                  const PluginCollector::Extensions& extensions,
                                  PathMap& paths);

PathEntry matchPath(const std::filesystem::path& path,
                    const PluginCollector::Extensions& extensions);

std::vector<std::string> split(std::string_view text);

}

void PluginCollector::addPluginExtension(std::string_view extension)
{
  extensions.emplace_back(extension);
}

void PluginCollector::addDirectory(std::string_view dir)
{
  dirs.emplace_back(dir);
}

void PluginCollector::addDirectories(const Directories& dirs)
{
  this->dirs.insert(this->dirs.end(), dirs.begin(), dirs.end());
}

void PluginCollector::enablePathEnvVariable(std::string_view name)
{
  pathEnvVariableName = name;
}

void PluginCollector::enableFileEnvVariable(std::string_view name)
{
  fileEnvVariableName = name;
}

PluginCollector::Paths PluginCollector::collectPlugins()
{
  Paths paths;
  PathMap pathMap;

  if (!fileEnvVariableName.empty()) {
    appendEnvVariableFiles(paths);
  }

  if (!pathEnvVariableName.empty()) {
    addEnvVariableDirectories();
  }

  appendDirectoriesScanResults(dirs, extensions, pathMap);

  transform(pathMap.begin(), pathMap.end(), std::back_inserter(paths),
            [](const PathMap::value_type& val){return val.second.path;} );

  return paths;
}

void PluginCollector::appendEnvVariableFiles(Paths& paths)
{
  auto envValue = std::getenv(fileEnvVariableName.c_str());
  if (envValue) {
    auto files = split(envValue);
    paths.insert(paths.end(), files.begin(), files.end());
  }
}

void PluginCollector::addEnvVariableDirectories()
{
  auto envValue = std::getenv(pathEnvVariableName.c_str());
  if (envValue) {
    auto dirs = split(envValue);
    for (const auto& dir: dirs) {
      addDirectory(dir);
    }
  }
}


// -------------------

namespace {

void appendDirectoriesScanResults(PluginCollector::Directories& dirs,
                                  const PluginCollector::Extensions& extensions,
                                  PathMap& paths)
{
  for (const auto& dir: dirs) {
    if (!std::filesystem::exists(dir)) {
      continue;
    }
    std::filesystem::directory_iterator dirIt(dir);
    for (const auto& path: dirIt) {
      auto pathEntry = matchPath(path, extensions);
      if (pathEntry.valid) {
        auto it = paths.find(pathEntry.name);
        if (it != paths.end()) {
          if (it->second.version > pathEntry.version) {
            continue;
          }
          it->second = pathEntry;
        }
        else {
          paths.insert(std::make_pair(pathEntry.name, pathEntry));
        }
      }
    }
  }
}

PathEntry matchPath(const std::filesystem::path& path,
                    const PluginCollector::Extensions& extensions)
{
  PathEntry entry;
  for (const auto& extension: extensions) {
    std::regex rgx(R"((.+\.)" + extension + R"()(\.[.0-9]*)?$)");
    std::smatch matches;
    auto filename = path.filename().string();
    if (std::regex_search(filename, matches, rgx)) {
      entry.valid = true;
      entry.path = path;
      entry.name = matches[NAME_MATCH_INDEX];
      if (matches.size() == FULL_VERSION_MATCH_SIZE) {
        entry.version = matches[VERSION_MATCH_INDEX];
      }
      break;
    }

  }
  return entry;
}

std::vector<std::string> split(std::string_view text)
{
  std::vector<std::string> vector;
  std::stringstream stream(text.data());
  std::string item;
  while(std::getline(stream, item, ';')) {
      vector.push_back(item);
  }
  return vector;
}

} // namespace


