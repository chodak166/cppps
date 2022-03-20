// Copyright (c) 2021  Lukasz Chodyla
// Distributed under the MIT License.
// See accompanying file LICENSE.txt for the full license.

#ifndef WINDOWSPLUGINLOADER_H
#define WINDOWSPLUGINLOADER_H

#include <cppps/dl/IPluginLoader.h>

namespace cppps {

class WindowsPluginLoader: public IPluginLoader
{
public:
  cppps::IPluginDPtr load(std::string_view path) override;
};


} // namespace cppps


#endif // WINDOWSPLUGINLOADER_H
