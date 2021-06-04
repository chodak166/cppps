// Copyright (c) 2020  Lukasz Chodyla
// Distributed under the Boost Software License v1.0.
// See accompanying file LICENSE.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt for the full license.

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
