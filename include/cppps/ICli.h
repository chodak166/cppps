// Copyright (c) 2020  Lukasz Chodyla
// Distributed under the Boost Software License v1.0.
// See accompanying file LICENSE.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt for the full license.

#ifndef ICLI_H
#define ICLI_H

#include <memory>

namespace cppps {

class ICli
{
public:
  virtual ~ICli() = default;

};

using ICliPtr = std::shared_ptr<ICli>;

} // namespace cppps

#endif // ICLI_H
