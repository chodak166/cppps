#ifndef VERSION_H_IN
#define VERSION_H_IN

#include <string>

namespace cppps::minex {

constexpr char APP_NAME[] = "cppps-minimal";
constexpr char APP_DESCRIPTION[] = "CPPPS minimal example";
constexpr int VERSION_MAJOR = 1;
constexpr int VERSION_MINOR = 0;
constexpr int VERSION_PATCH = 0;
constexpr char VERSION_SUFFIX[] = "";

inline std::string getVersionString() {
  return std::to_string(VERSION_MAJOR)
      + "." + std::to_string(VERSION_MINOR)
      + "." + std::to_string(VERSION_PATCH)
      + VERSION_SUFFIX;
}

} // namespace cppps

#endif // VERSION_H_IN
