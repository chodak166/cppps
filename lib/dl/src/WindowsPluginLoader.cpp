#include "WindowsPluginLoader.h"
#include "cppps/dl/exceptions.h"
#include <windows.h>


using cppps::WindowsPluginLoader;
using cppps::IPlugin;
using cppps::IPluginDPtr;
using cppps::IPluginUPtr;

namespace {

class Library
{
public:
  Library(const std::string& path)
  {
    handle = LoadLibrary(path.c_str());
    if (!handle) {
      throw cppps::PluginNotFoundException("Cannot load library: " + path);
    }
  }

  template <class T>
  T importSymbol(const std::string& symbol)
  {
    auto rawSymbol = (void*(*)())GetProcAddress(handle, symbol.c_str());
    if (!rawSymbol) {
      throw cppps::MakePluginNotFoundException("Cannot load symbol (" + symbol + ")");
    }

    auto targetSymbol = reinterpret_cast<T>(rawSymbol());
    return targetSymbol;
  }

  void unload() noexcept
  {
    if (handle) {
      FreeLibrary(handle);
      handle = nullptr;
    }
  }

  ~Library()
  {
    unload();
  }

private:
  using LibraryHandle = HMODULE;

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

cppps::IPluginDPtr WindowsPluginLoader::load(std::string_view path)
{
  auto lib = std::make_shared<Library>(path.data());
  auto makePlugin = lib->importSymbol<IPluginUPtr(*)()>("make_plugin");
  return bindPlugin(makePlugin(), lib);
}
