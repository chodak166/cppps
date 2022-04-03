cmake_minimum_required(VERSION 3.16)

project(cppps LANGUAGES CXX)

get_filename_component(CPPPS_ROOT ${CMAKE_CURRENT_LIST_DIR} ABSOLUTE)

enable_testing()

option(CPPPS_DL_BUILD "Build CPPPS DL library" ON)
if (${CPPPS_DL_BUILD})
  add_subdirectory(lib/dl)
endif()

option(CPPPS_LOGGING_BUILD "Build CPPPS logging library" ON)
if (${CPPPS_LOGGING_BUILD})
  add_subdirectory(lib/logging)
endif()
