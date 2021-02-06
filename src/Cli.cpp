// Copyright (c) 2020  Lukasz Chodyla
// Distributed under the Boost Software License v1.0.
// See accompanying file LICENSE.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt for the full license.

#include "cppps/Cli.h"
#include <CLI/CLI.hpp>

using cppps::Cli;

namespace {

constexpr int VERSION_REQUEST_CODE = -1;

void setupStandardOptions(CLI::App& cliApp)
{
  //TODO: use CallForVersion
  cliApp.add_flag_function("-v,--version", [](size_t) {
      throw CLI::CallForHelp("Version request found", VERSION_REQUEST_CODE);
    }, "Print application version");

  cliApp.set_config("-c,--config");
}

}

struct Cli::CliPimpl
{
  CLI::App cliApp;
};

// -----------------------

Cli::Cli(const AppInfo& appInfo)
  : appInfo{appInfo}
  , pimpl{std::make_unique<CliPimpl>()}
{
  setupStandardOptions(pimpl->cliApp);
}

Cli::~Cli() = default;

void Cli::parse(int argc, char* argv[])
{
  auto& cliApp = pimpl->cliApp;
  cliApp.App::name(appInfo.appName);
  cliApp.App::description(appInfo.appDescription);
  cliApp.get_formatter()->column_width(appInfo.columnWidth);

  try {
      cliApp.parse(argc, argv);
  } catch(const CLI::CallForHelp &e) {
    message = (e.get_exit_code() == VERSION_REQUEST_CODE)
        ? appInfo.appVersionPage + '\n'
        : cliApp.help();
  }
}

bool Cli::shouldAppQuit() const
{
  return hasMessage();
}

bool Cli::hasMessage() const
{
  return !message.empty();
}

const std::string& Cli::getMessage() const
{
  return message;
}

void Cli::addOption(std::string_view option, std::string& target,
                         std::string_view description, bool defaulted)
{
  pimpl->cliApp.add_option(option.data(), target,
                             description.data(), defaulted);
}

void Cli::addOption(std::string_view option, int& target,
                         std::string_view description, bool defaulted)
{
  pimpl->cliApp.add_option(option.data(), target,
                             description.data(), defaulted);
}

void Cli::addOption(std::string_view option, uint8_t& target,
                         std::string_view description, bool defaulted)
{
  pimpl->cliApp.add_option(option.data(), target,
                             description.data(), defaulted);
}

void Cli::addOption(std::string_view option, uint16_t& target,
                         std::string_view description, bool defaulted)
{
  pimpl->cliApp.add_option(option.data(), target,
                             description.data(), defaulted);
}

void Cli::addOption(std::string_view option, uintmax_t& target,
                         std::string_view description, bool defaulted)
{
  pimpl->cliApp.add_option(option.data(), target,
                             description.data(), defaulted);
}

void Cli::addOption(std::string_view option, float& target,
                         std::string_view description, bool defaulted)
{
  pimpl->cliApp.add_option(option.data(), target,
                             description.data(), defaulted);
}

void Cli::addOption(std::string_view option, double& target,
                         std::string_view description, bool defaulted)
{
  pimpl->cliApp.add_option(option.data(), target,
                             description.data(), defaulted);
}

void Cli::addFlag(std::string_view option, bool& target,
                       std::string_view description)
{
  pimpl->cliApp.add_flag(option.data(), target,
                           description.data());
}

void Cli::addOption(std::string_view option, std::vector<long int>& target,
                         std::string_view description, bool defaulted)
{
  pimpl->cliApp.add_option(option.data(), target,
                           description.data(), defaulted);
}

void Cli::addOption(std::string_view option, std::vector<double>& target,
                    std::string_view description, bool defaulted)
{
  pimpl->cliApp.add_option(option.data(), target,
                           description.data(), defaulted);
}

void Cli::addOption(std::string_view option, std::vector<std::string>& target,
                    std::string_view description, bool defaulted)
{
  pimpl->cliApp.add_option(option.data(), target,
                           description.data(), defaulted);
}
