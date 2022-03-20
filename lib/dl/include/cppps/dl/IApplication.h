// Copyright (c) 2021  Lukasz Chodyla
// Distributed under the MIT License.
// See accompanying file LICENSE.txt for the full license.

#ifndef IAPPLICATION_H
#define IAPPLICATION_H

#include <functional>

namespace cppps {

class IApplication
{
public:

  using MainLoop = std::function<int()>;

  virtual ~IApplication() = default;
  virtual void setMainLoop(const MainLoop& loop) = 0;
  virtual void quit() = 0;

};

} // namespace cppps

#endif // IAPPLICATION_H
