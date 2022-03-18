// Copyright (c) 2020  Lukasz Chodyla
// Distributed under the Boost Software License v1.0.
// See accompanying file LICENSE.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt for the full license.

#ifndef IPRODUCT_H
#define IPRODUCT_H

#include <string>
#include <memory>

class IProduct
{
public:
  virtual ~IProduct() = default;
  virtual int getValue() = 0;
  virtual int increaseValue() = 0;
};

using IProductPtr = std::shared_ptr<IProduct>;
using IProductWPtr = std::weak_ptr<IProduct>;

#endif // IPRODUCT_H
