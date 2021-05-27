cmake_minimum_required(VERSION 3.5)

project(cppps LANGUAGES CXX)


option(CPPPS_DL_BUILD "Build CPPPS DL library" ON)
if (${CPPPS_DL_BUILD})
  add_subdirectory(lib/dl)
endif()

