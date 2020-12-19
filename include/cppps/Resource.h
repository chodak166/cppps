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

class TypeMismatchException: public std::runtime_error {
  using runtime_error::runtime_error;
};

class BadCastException: public std::runtime_error {
  using runtime_error::runtime_error;
};

/**
 * @brief A simple std::any wrapper that
 * introduces custom exception and "as" method.
 *
 */
class Resource
{
public:
  Resource() = delete;

  template<class T>
  Resource(T&& value);

  template<class T>
  T as() const;

private:
  std::any object;
};

using ResourceProvider = std::function<Resource()>;
using ResourceConsumer = std::function<void(const Resource&)>;

// ----------

template<class T>
Resource::Resource(T&& value)
{
  object = std::move(value);
}

template<class T>
T Resource::as() const
{
  if (typeid(T) != object.type()) {
    throw TypeMismatchException(std::string("Resource conversion not allowed: ")
                             + object.type().name() + " to " + typeid(T).name());
  }
  try {
    return std::any_cast<T>(object);
  }
  catch(std::bad_any_cast& e) {
    throw BadCastException(std::string("Resource type conversion failed: ")
                           + object.type().name() + " to " + typeid(T).name()
                           + "; ensure that RTLD_GLOBAL flag is enabled");
  }
}

} // namespace cppps

#endif // RESOURCE_H
