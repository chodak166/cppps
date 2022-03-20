// Copyright (c) 2020  Lukasz Chodyla
// Distributed under the MIT License.
// See accompanying file LICENSE.txt for the full license.

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
