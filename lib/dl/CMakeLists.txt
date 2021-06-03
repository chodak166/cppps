cmake_minimum_required(VERSION 3.5)

project(cppps-dl LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/lib)

get_filename_component(PROJECT_ROOT ../../ ABSOLUTE)

include("${PROJECT_ROOT}/cmake/helpers.cmake")
list(APPEND CMAKE_MODULE_PATH "${PROJECT_ROOT}/cmake/modules")

find_package(Boost REQUIRED COMPONENTS filesystem system) #TODO make boost optional
find_package(CLI MODULE)

add_library(${PROJECT_NAME} SHARED
  src/Application.cpp
  src/BoostPluginLoader.cpp
  src/Cli.cpp
  src/PluginCollector.cpp
  src/PluginSystem.cpp
  )
add_library(cppps::dl ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
  PUBLIC
  include
  ${CMAKE_CURRENT_BINARY_DIR}/include

  PRIVATE
  src
  ${CPPPS_CLI_INCLUDE_DIR}
  )

set_target_properties(${PROJECT_NAME} PROPERTIES
  ENABLE_EXPORTS ON
  )

target_link_libraries(${PROJECT_NAME}
  ${Boost_LIBRARIES}
  dl
  stdc++fs
  )

enable_testing(ON)

set(LIB_ROOT ${CMAKE_CURRENT_LIST_DIR})

option(CPPPS_UNIT_TESTS_BUILD "Build CPPPS unit tests")
if (${CPPPS_UNIT_TESTS_BUILD})
  add_subdirectory(tests/unit)
endif()

option(CPPPS_INTEGRATION_TESTS_BUILD "Build CPPPS integraton tests")
if (${CPPPS_INTEGRATION_TESTS_BUILD})
  add_subdirectory(tests/integration)
endif()

