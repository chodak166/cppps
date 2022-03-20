// Copyright (c) 2020  Lukasz Chodyla
// Distributed under the MIT License.
// See accompanying file LICENSE.txt for the full license.

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
