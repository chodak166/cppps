#include "DummyPlugin.h"
#include <boost/dll/alias.hpp>

cppps::IPluginUPtr makePlugin()
{
  auto plugin = std::make_unique<DummyPlugin>(PLUGIN_NAME, PLUGIN_VERSION);
  return plugin;
}

BOOST_DLL_ALIAS(makePlugin, make_plugin)
