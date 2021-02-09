set(CLI_DIR "${PROJECT_SOURCE_DIR}/submodules/cli11" CACHE STRING "CLI11 root directory")

set(CLI_PATHS
  ${CLI_DIR}
  $ENV{CLI_DIR}
  "${EXTERN_DIR}/cli11"
  "${PROJECT_SOURCE_DIR}/extern/cli11"
  "${PROJECT_SOURCE_DIR}/external/cli11"
  "${PROJECT_SOURCE_DIR}/submodules/cli11"
  "${CMAKE_SOURCE_DIR}/extern/cli11"
  "${CMAKE_SOURCE_DIR}/external/cli11"
  "${CMAKE_SOURCE_DIR}/submodules/cli11"
  )

find_path(CPPPS_CLI_INCLUDE_DIR
  NAMES "CLI/CLI.hpp"
  PATH_SUFFIXES include src
  PATHS ${CLI_PATHS}
  )

if (NOT CPPPS_CLI_INCLUDE_DIR)
  message(FATAL_ERROR "CLI11 library has not been found. Consider using CLI_DIR variable." )
endif()

find_package_handle_standard_args(CLI REQUIRED_VARS CPPPS_CLI_INCLUDE_DIR)
