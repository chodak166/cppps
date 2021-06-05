// Copyright (c) 2021  Lukasz Chodyla
// Distributed under the Boost Software License v1.0.
// See accompanying file LICENSE.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt for the full license.

#include "DlopenPluginLoader.h"
#include "cppps/dl/exceptions.h"
#include <dlfcn.h>

using cppps::DlopenPluginLoader;
using cppps::IPlugin;
using cppps::IPluginDPtr;
using cppps::IPluginUPtr;

namespace {

class Library
{
public:
  Library(const std::string& path)
  {
    handle = dlopen(path.c_str(), RTLD_NOW | RTLD_GLOBAL);
    if (!handle) {
      throw cppps::PluginNotFoundException("Cannot load library: " + std::string(dlerror()));
    }
  }

  template <class T>
  T importSymbol(const std::string& symbol)
  {
    auto rawSymbol = (void**)dlsym(handle, symbol.c_str());
    const char* dlsym_error = dlerror();
    if (dlsym_error) {
      throw cppps::MakePluginNotFoundException("Cannot load symbol (" + symbol + "): " + dlsym_error);
    }

    return reinterpret_cast<T>(*rawSymbol);
  }

  void unload() noexcept
  {
    if (handle) {
      dlclose(handle);
      handle = nullptr;
    }
  }

  ~Library()
  {
    unload();
  }

private:
  using LibraryHandle = void*;

  LibraryHandle handle {nullptr};
};

struct PluginDeleter
{
  using LibType = Library;
  using LibTypePtr = std::shared_ptr<LibType>;
  PluginDeleter(const LibTypePtr& library)
    : library{library}{};

  void operator()(IPlugin* object);

private:
  LibTypePtr library {nullptr};
};

void PluginDeleter::operator()(IPlugin* object)
{
  if (object) {
    delete object;
    object = nullptr;
  }
}

IPluginDPtr bindPlugin(IPluginUPtr&& plugin, PluginDeleter::LibTypePtr& lib)
{
  auto rawPlugin = plugin.get();
  plugin.release();
  auto bindedPtr = IPluginDPtr(rawPlugin, PluginDeleter(lib));
  return bindedPtr;
}

}

cppps::IPluginDPtr DlopenPluginLoader::load(std::string_view path)
{
  auto lib = std::make_shared<Library>(path.data());
  auto makePlugin = lib->importSymbol<IPluginUPtr(*)()>("make_plugin");
  return bindPlugin(makePlugin(), lib);
}
