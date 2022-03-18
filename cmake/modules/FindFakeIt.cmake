set(FAKEIT_DIR "${PROJECT_SOURCE_DIR}/submodules/fakeit" CACHE STRING "FakeIt root directory")

set(FAKEIT_PATHS
  ${FAKEIT_DIR}
  $ENV{FAKEIT_DIR}
  "${EXTERN_DIR}/fakeit"
  "${PROJECT_SOURCE_DIR}/extern/fakeit"
  "${PROJECT_SOURCE_DIR}/external/fakeit"
  "${PROJECT_SOURCE_DIR}/submodules/fakeit"
  "${CMAKE_SOURCE_DIR}/extern/fakeit"
  "${CMAKE_SOURCE_DIR}/external/fakeit"
  "${CMAKE_SOURCE_DIR}/submodules/fakeit"
  "${CPPPS_ROOT}/submodules/fakeit"
  )

find_path(CPPPS_FAKEIT_INCLUDE_DIR
  NAMES "catch/fakeit.hpp"
  PATH_SUFFIXES single_header
  PATHS ${FAKEIT_PATHS}
  )

if (NOT CPPPS_FAKEIT_INCLUDE_DIR)
  message(FATAL_ERROR "FakeIt library has not been found. Consider using FAKEIT_DIR variable." )
endif()

find_package_handle_standard_args(FakeIt REQUIRED_VARS CPPPS_FAKEIT_INCLUDE_DIR)
