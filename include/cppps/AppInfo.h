// Copyright (c) 2020  Lukasz Chodyla
// Distributed under the Boost Software License v1.0.
// See accompanying file LICENSE.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt for the full license.

#ifndef APPINFO_H
#define APPINFO_H

#include <string>

namespace cppps {

struct AppInfo
{
  std::string appName;
  std::string appDescription;
  std::string appVersionPage;
  int columnWidth {40};
  bool interruptable {true};
};

} // namespace cppps

#endif // APPINFO_H
