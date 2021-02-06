cmake_minimum_required(VERSION 3.5)

project(cppps LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/lib)

set(PROJECT_ROOT ${PROJECT_SOURCE_DIR})

include(cmake/helpers.cmake)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost REQUIRED COMPONENTS filesystem system)

add_library(${PROJECT_NAME}
  src/Application.cpp
  src/BoostPluginLoader.cpp
  src/Cli.cpp
  src/PluginCollector.cpp
  src/PluginSystem.cpp
  )

target_include_directories(${PROJECT_NAME}
  PUBLIC
  include

  PRIVATE
  src
  submodules/cli11/include
  )

#set_target_properties(${PROJECT_NAME} PROPERTIES
#  CXX_VISIBILITY_PRESET hidden)


target_link_libraries(${PROJECT_NAME}
  ${Boost_LIBRARIES}
  dl
  stdc++fs
  )

enable_testing(ON)

option(CPPPS_UNIT_TESTS_BUILD "Build CPPPS unit tests")
if (${CPPPS_UNIT_TESTS_BUILD})
  add_subdirectory(tests/unit)
endif()

option(CPPPS_INTEGRATION_TESTS_BUILD "Build CPPPS integraton tests")
if (${CPPPS_INTEGRATION_TESTS_BUILD})
  add_subdirectory(tests/integration)
endif()

