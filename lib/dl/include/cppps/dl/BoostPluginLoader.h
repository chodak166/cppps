// Copyright (c) 2020  Lukasz Chodyla
// Distributed under the Boost Software License v1.0.
// See accompanying file LICENSE.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt for the full license.

#ifndef BOOSTPLUGINLOADER_H
#define BOOSTPLUGINLOADER_H

#include "cppps/dl/IPluginLoader.h"

namespace cppps {

class BoostPluginLoader: public IPluginLoader
{
public:
  cppps::IPluginDPtr load(std::string_view path) override;
};

} // namespace cppps

#endif // BOOSTPLUGINLOADER_H
