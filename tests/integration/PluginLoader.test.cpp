#include <catch2/catch.hpp>
//#include <fakeit/catch/fakeit.hpp>

#include <boost/dll/runtime_symbol_info.hpp>
#include <boost/dll/shared_library.hpp>
#include <boost/function.hpp>
#include "cppps/IPlugin.h"

//NOTE: use -Wno-disabled-macro-expansion to suppress fakeit macro warnings
//using namespace fakeit;

namespace {

template <class T>
struct BindedLibDeleter
{
  using LibType = boost::dll::shared_library;
  using LibTypePtr = std::shared_ptr<LibType>;
  BindedLibDeleter(const LibTypePtr& library)
    : library{library}{};

  void operator()(T* object)
  {
    if (object) {
      delete object;
    }
  }

private:
  LibTypePtr library {nullptr};
};

using PluginDeleter = BindedLibDeleter<cppps::IPlugin>;
using BindedPluginUPtr = std::unique_ptr<cppps::IPlugin, PluginDeleter>;

BindedPluginUPtr bind(cppps::IPluginUPtr&& plugin, PluginDeleter::LibTypePtr& lib)
{
  auto rawPlugin = plugin.get();
  plugin.release();
  auto bindedPtr = BindedPluginUPtr(rawPlugin, PluginDeleter(lib));
  return bindedPtr;
}

}


class PluginLoader
{
public:
  BindedPluginUPtr load(std::string_view path)
  {
    auto lib = std::make_shared<boost::dll::shared_library>(path.data(),
                                                            boost::dll::load_mode::append_decorations);
    auto makePlugin = lib->get_alias<cppps::IPluginUPtr()>("make_plugin");
    auto plugin = bind(makePlugin(), lib);
    return plugin;
  }
};


namespace test {
namespace {

const auto PLUGIN_DIR = boost::dll::program_location().parent_path().string() + "/plugins";
constexpr auto PLUGIN_A_NAME ="PLUGIN A";
const auto PLUGIN_A_PATH = PLUGIN_DIR + "/libplugin_a.so"; //TODO: .dll?


} // namespace
} // namespace test



TEST_CASE("Testing plugin loader", "[plugin_loader]")
{
  PluginLoader loader;

  SECTION("When a single plugin is loaded, then the returned object is not null")
  {
    auto plugin = loader.load(test::PLUGIN_A_PATH);
    REQUIRE(plugin != nullptr);
  }

  SECTION("When a single plugin is loaded, then the interface methods can be invoked")
  {
    auto plugin = loader.load(test::PLUGIN_A_PATH);
    auto name = plugin->getName();
    REQUIRE(name == test::PLUGIN_A_NAME);
  }

  SECTION("When the loader is destroyed, then all the loaded plugins remains valid")
  {
    BindedPluginUPtr plugin {nullptr, PluginDeleter{nullptr}};

    {
      PluginLoader localLoader;
      plugin = localLoader.load(test::PLUGIN_A_PATH);
    }

    auto name = plugin->getName();
    REQUIRE(name == test::PLUGIN_A_NAME);
  }

  //TODO: multiple plugins
}

//TODO: exceptions

