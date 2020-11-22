#include "Plugin.h"

//#include <boost/dll/runtime_symbol_info.hpp>
#include <iostream>

Plugin::Plugin()
{

}

Plugin::~Plugin()
{
  std::cout << "Destroying " << getName()
            << " plugin!" << std::endl;
}

std::string Plugin::getLocation() const
{
  //return boost::dll::this_line_location().string();
  return "";
}

void Plugin::load()
{
  std::cout << getName() << " plugin loaded!" << std::endl;
}
