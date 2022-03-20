// Copyright (c) 2020  Lukasz Chodyla
// Distributed under the MIT License.
// See accompanying file LICENSE.txt for the full license.

#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>

namespace cppps {

class PluginNotFoundException: public std::runtime_error {
  using runtime_error::runtime_error;};

class MakePluginNotFoundException: public std::runtime_error {
  using runtime_error::runtime_error;};

class UnresolvedDependencyException: public std::runtime_error {
  using runtime_error::runtime_error;
};

class CircularDependencyException: public std::runtime_error {
  using runtime_error::runtime_error;
};

} // namespace cppps


#endif // EXCEPTIONS_H
