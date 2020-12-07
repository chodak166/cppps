// Copyright (c) 2020  Lukasz Chodyla
// Distributed under the Boost Software License v1.0.
// See accompanying file LICENSE.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt for the full license.

#ifndef RESOURCE_H
#define RESOURCE_H

#include <memory>
#include <functional>
#include <any>

namespace cppps {

class Resource
{
public:
  Resource() = delete;

  template<class T>
  Resource(T&& value)
  {
    object = std::move(value);
  }

  template<class T>
  T as() const
  {
    if (typeid(T) != object.type()) {
      throw std::runtime_error(std::string("Resource conversion not allowed: ")
                               + object.type().name() + " to " + typeid(T).name());
    }
    return std::any_cast<T>(object);
  }

private:
  std::any object;
};

using ResourceProvider = std::function<Resource()>;
using ResourceConsumer = std::function<void(const Resource&)>;

} // namespace cppps

#endif // RESOURCE_H
