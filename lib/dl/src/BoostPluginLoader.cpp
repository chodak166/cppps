// Copyright (c) 2020  Lukasz Chodyla
// Distributed under the Boost Software License v1.0.
// See accompanying file LICENSE.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt for the full license.

#include "cppps/dl/BoostPluginLoader.h"
#include "cppps/dl/IPlugin.h"
#include "cppps/dl/exceptions.h"

#include <filesystem>

#include <boost/dll/runtime_symbol_info.hpp>
#include <boost/dll/shared_library.hpp>
#include <boost/function.hpp>
#include <boost/exception/diagnostic_information.hpp>

#include <iostream>

using cppps::BoostPluginLoader;
using cppps::IPluginUPtr;
using cppps::IPluginDPtr;

namespace {

struct PluginDeleter
{
  using LibType = boost::dll::shared_library;
  using LibTypePtr = std::shared_ptr<LibType>;
  PluginDeleter(const LibTypePtr& library)
    : library{library}{};

  void operator()(cppps::IPlugin* object);

private:
  LibTypePtr library {nullptr};
};

IPluginDPtr bind(IPluginUPtr&& plugin, PluginDeleter::LibTypePtr& lib);

} // namespace


IPluginDPtr BoostPluginLoader::load(std::string_view path)
{
  if (!std::filesystem::exists(path)) {
    throw cppps::PluginNotFoundException(std::string("Plugin file not found: ") + path.data());
  }

  std::function<cppps::IPluginUPtr()> makePlugin {nullptr};
  std::shared_ptr<boost::dll::shared_library> library {nullptr};

  try {
    library = std::make_shared<boost::dll::shared_library>(path.data(),
                                                           boost::dll::load_mode::append_decorations
                                                           | boost::dll::load_mode::rtld_global);
    makePlugin = library->get_alias<cppps::IPluginUPtr()>("make_plugin");
  }
  catch (boost::system::system_error& e) {
    throw cppps::MakePluginNotFoundException(
          std::string("The required symbol 'make_plugin' has not been found in ")
          + path.data() + " (" + e.what() + ")");
  }

  auto plugin = bind(makePlugin(), library);
  return plugin;
}

// ------------------

namespace  {

void PluginDeleter::operator()(cppps::IPlugin* object)
{
  if (object) {
    delete object;
    object = nullptr;
  }
}

IPluginDPtr bind(IPluginUPtr&& plugin, PluginDeleter::LibTypePtr& lib)
{
  auto rawPlugin = plugin.get();
  plugin.release();
  auto bindedPtr = IPluginDPtr(rawPlugin, PluginDeleter(lib));
  return bindedPtr;
}

} // namespace
