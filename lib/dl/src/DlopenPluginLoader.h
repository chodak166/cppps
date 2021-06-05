// Copyright (c) 2021  Lukasz Chodyla
// Distributed under the Boost Software License v1.0.
// See accompanying file LICENSE.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt for the full license.

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
