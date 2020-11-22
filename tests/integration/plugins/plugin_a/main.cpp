#include "Plugin.h"
#include <boost/dll/alias.hpp>

// Exporting `my_namespace::plugin` variable with alias name `plugin`
// (Has the same effect as `BOOST_DLL_ALIAS(my_namespace::plugin, plugin)`)
//extern "C" BOOST_SYMBOL_EXPORT cppps::IPluginPtr makePlugin();

cppps::IPluginUPtr makePlugin()
{
  auto plugin = std::make_unique<Plugin>();
  return plugin;
}

BOOST_DLL_ALIAS(
    makePlugin,   // <-- this function is exported with...
    make_plugin   // <-- ...this alias name
)

