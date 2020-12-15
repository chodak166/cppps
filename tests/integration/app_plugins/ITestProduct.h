// Copyright (c) 2020  Lukasz Chodyla
// Distributed under the Boost Software License v1.0.
// See accompanying file LICENSE.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt for the full license.

#ifndef ITESTPRODUCT_H
#define ITESTPRODUCT_H

#include <string>
#include <memory>

class ITestProduct
{
public:
  virtual ~ITestProduct() = default;
  virtual std::string getValue() const = 0;
};

using ITestProductPtr = std::shared_ptr<ITestProduct>;
using ITestProductWPtr = std::weak_ptr<ITestProduct>;

#endif // ITESTPRODUCT_H
