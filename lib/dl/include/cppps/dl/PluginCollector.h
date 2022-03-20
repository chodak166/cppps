// Copyright (c) 2020  Lukasz Chodyla
// Distributed under the MIT License.
// See accompanying file LICENSE.txt for the full license.

#ifndef PLUGINCOLLECTOR_H
#define PLUGINCOLLECTOR_H

#include <list>
#include <string>

namespace cppps {

class PluginCollector
{
public:

  using Paths = std::list<std::string>;
  using Directories = std::list<std::string>;
  using Extensions = std::list<std::string>;

  virtual ~PluginCollector() = default;
  void addPluginExtension(std::string_view extension);
  void addDirectory(std::string_view dir);
  void addDirectories(const Directories& dirs);
  void enablePathEnvVariable(std::string_view name);
  void enableFileEnvVariable(std::string_view name);
  Paths collectPlugins();


private:
  std::list<std::string> extensions;
  std::list<std::string> dirs;
  std::string pathEnvVariableName;
  std::string fileEnvVariableName;

private:
  void appendEnvVariableFiles(Paths& paths);
  void addEnvVariableDirectories();

};

} // namespace cppps


#endif // PLUGINCOLLECTOR_H
