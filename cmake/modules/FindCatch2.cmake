set(CATCH2_DIR "${PROJECT_SOURCE_DIR}/submodules/catch2" CACHE STRING "FakeIt root directory")

set(CATCH2_PATHS
  ${CATCH2_DIR}
  $ENV{CATCH2_DIR}
  "${EXTERN_DIR}/catch2"
  "${PROJECT_SOURCE_DIR}/extern/catch2"
  "${PROJECT_SOURCE_DIR}/external/catch2"
  "${PROJECT_SOURCE_DIR}/submodules/catch2"
  "${CMAKE_SOURCE_DIR}/extern/catch2"
  "${CMAKE_SOURCE_DIR}/external/catch2"
  "${CMAKE_SOURCE_DIR}/submodules/catch2"
  )

find_path(CPPPS_CATCH2_INCLUDE_DIR
  NAMES "catch2/catch.hpp"
  PATH_SUFFIXES single_include
  PATHS ${CATCH2_PATHS}
  )

if (NOT CPPPS_CATCH2_INCLUDE_DIR)
  message(FATAL_ERROR "Catch2 library has not been found. Consider using CATCH2_DIR variable." )
endif()

find_package_handle_standard_args(Catch2 REQUIRED_VARS CPPPS_CATCH2_INCLUDE_DIR)
