// Copyright (c) 2020  Lukasz Chodyla
// Distributed under the Boost Software License v1.0.
// See accompanying file LICENSE.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt for the full license.

#ifndef APPCLI_H
#define APPCLI_H

#include "cppps/ICli.h"
#include "cppps/AppInfo.h"

#include <memory>
#include <vector>

namespace cppps {

class Cli: public ICli
{
public:

  explicit Cli(const AppInfo& appInfo);
  virtual ~Cli();

  void parse(int argc, char* argv[]);

  [[nodiscard]] bool shouldAppQuit() const;

  [[nodiscard]] bool hasMessage() const;

  [[nodiscard]] const std::string& getMessage() const;

  // ICli interface
  void addOption(std::string_view option, std::string& target,
                 std::string_view description, bool defaulted = true) override;
  void addOption(std::string_view option, int& target,
                 std::string_view description, bool defaulted = true) override;
  void addOption(std::string_view option, uint8_t& target,
                 std::string_view description, bool defaulted = true) override;
  void addOption(std::string_view option, uint16_t& target,
                 std::string_view description, bool defaulted = true) override;
  void addOption(std::string_view option, uintmax_t& target,
                 std::string_view description, bool defaulted = true) override;
  void addOption(std::string_view option, float& target,
                 std::string_view description, bool defaulted = true) override;
  void addOption(std::string_view option, double& target,
                 std::string_view description, bool defaulted = true) override;
  void addOption(std::string_view option, std::vector<long>& target
                 , std::string_view description, bool defaulted = true) override;
  void addOption(std::string_view option, std::vector<double>& target,
                 std::string_view description, bool defaulted = true) override;
  void addOption(std::string_view option, std::vector<std::string>& target,
                 std::string_view description, bool defaulted = true) override;
  void addFlag(std::string_view option, bool& target,
               std::string_view description) override;

private:
  AppInfo appInfo;
  std::string message;

  struct CliPimpl;
  std::unique_ptr<CliPimpl> pimpl;

};

using CliPtr = std::shared_ptr<Cli>;

} // namespace cppps

#endif // APPCLI_H
