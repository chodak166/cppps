// Copyright (c) 2021  Lukasz Chodyla
// Distributed under the MIT License.
// See accompanying file LICENSE.txt for the full license.

#ifndef DLOPENPLUGINLOADER_H
#define DLOPENPLUGINLOADER_H

#include <cppps/dl/IPluginLoader.h>

namespace cppps {

class DlopenPluginLoader: public IPluginLoader
{
public:
  cppps::IPluginDPtr load(std::string_view path) override;
};


} // namespace cppps

#endif // DLOPENPLUGINLOADER_H
