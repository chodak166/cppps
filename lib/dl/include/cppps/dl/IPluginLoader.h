// Copyright (c) 2020  Lukasz Chodyla
// Distributed under the MIT License.
// See accompanying file LICENSE.txt for the full license.

#ifndef IPLUGINLOADER_H
#define IPLUGINLOADER_H

#include "IPlugin.h"
#include <memory>

namespace cppps {

class IPluginLoader
{
public:
  virtual ~IPluginLoader() = default;
  virtual cppps::IPluginDPtr load(std::string_view path) = 0;
};

} // namespace cppps


#endif // IPLUGINLOADER_H
