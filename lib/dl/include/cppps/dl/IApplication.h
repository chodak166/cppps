// Copyright (c) 2021  Lukasz Chodyla
// Distributed under the Boost Software License v1.0.
// See accompanying file LICENSE.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt for the full license.

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
