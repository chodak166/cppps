cmake_minimum_required(VERSION 3.5)

project(cppps-examples LANGUAGES CXX)

option(CPPPS_EXAMPLE_BUILD_ALL "Build all CPPPS examples" OFF)

if (${CPPPS_EXAMPLE_BUILD_ALL})
  set(CPPPS_EXAMPLE_BUILD_MINIMAL ON CACHE BOOL "" FORCE)
  set(CPPPS_EXAMPLE_BUILD_LOGGER ON CACHE BOOL "" FORCE)
endif()

option(CPPPS_EXAMPLE_BUILD_MINIMAL "Build minimal CPPPS example" OFF)
if (${CPPPS_EXAMPLE_BUILD_MINIMAL})
  add_subdirectory(minimal)
endif()

option(CPPPS_EXAMPLE_BUILD_LOGGER "Build CPPPS shared logger example" OFF)
if (${CPPPS_EXAMPLE_BUILD_LOGGER})
  add_subdirectory(shared_logger)
endif()

# dummy target to be loaded by IDE
add_custom_target(cppps-examples)
