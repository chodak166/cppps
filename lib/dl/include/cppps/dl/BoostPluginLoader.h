// Copyright (c) 2020  Lukasz Chodyla
// Distributed under the MIT License.
// See accompanying file LICENSE.txt for the full license.

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
