// Copyright (c) 2020  Lukasz Chodyla
// Distributed under the MIT License.
// See accompanying file LICENSE.txt for the full license.

#include "OsUtils.h"

#include <filesystem>

#ifdef __linux
#include <unistd.h>
#include <linux/limits.h>
#else
#include <windows.h>
#endif

std::string cppps::getProgramDirPath()
{

#ifdef __unix
  char result[PATH_MAX];
  ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
  std::filesystem::path execPath = std::string(result, (count > 0) ? count : 0);
#else
  wchar_t path[_MAX_PATH] = { 0 };
  GetModuleFileNameW(NULL, path, MAX_PATH);
  std::filesystem::path execPath = path;
#endif

  return execPath.parent_path().string();
}
