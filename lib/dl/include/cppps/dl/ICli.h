// Copyright (c) 2020  Lukasz Chodyla
// Distributed under the MIT License.
// See accompanying file LICENSE.txt for the full license.

#ifndef ICLI_H
#define ICLI_H

#include <string>
#include <vector>
#include <memory>

namespace cppps {

class ICli
{
public:
  virtual ~ICli() = default;
  virtual void addOption(std::string_view option, std::string& target,
                 std::string_view description, bool defaulted = true) = 0;

  virtual void addOption(std::string_view option, int& target,
                 std::string_view description, bool defaulted = true) = 0;

  virtual void addOption(std::string_view option, uint8_t& target,
                 std::string_view description, bool defaulted = true) = 0;

  virtual void addOption(std::string_view option, uint16_t& target,
                 std::string_view description, bool defaulted = true) = 0;

  virtual void addOption(std::string_view option, uintmax_t& target,
                 std::string_view description, bool defaulted = true) = 0;

  virtual void addOption(std::string_view option, float& target,
                 std::string_view description, bool defaulted = true) = 0;

  virtual void addOption(std::string_view option, double& target,
                 std::string_view description, bool defaulted = true) = 0;

  virtual void addOption(std::string_view option, std::vector<long int>& target,
                 std::string_view description, bool defaulted = true) = 0;

  virtual void addOption(std::string_view option, std::vector<double>& target,
                 std::string_view description, bool defaulted = true) = 0;

  virtual void addOption(std::string_view option, std::vector<std::string>& target,
                 std::string_view description, bool defaulted = true) = 0;

  virtual void addFlag(std::string_view option, bool& target,
                 std::string_view description) = 0;

  virtual void removeOption(const std::string& name) = 0;

  //TODO: add option & flag callbacks
};

using ICliPtr = std::shared_ptr<ICli>;

} // namespace cppps

#endif // ICLI_H
