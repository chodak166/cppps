cmake_minimum_required(VERSION 3.5)

project(cppps LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/lib)

set(PROJECT_ROOT ${PROJECT_SOURCE_DIR})

include("${CMAKE_CURRENT_LIST_DIR}/cmake/helpers.cmake")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

find_package(Boost REQUIRED COMPONENTS filesystem system)
find_package(Logging MODULE)
find_package(CLI MODULE)

configure_file(src/Logging.h.in
  ${CMAKE_CURRENT_BINARY_DIR}/include/cppps/Logging.h)

add_library(${PROJECT_NAME} SHARED
  src/Application.cpp
  src/BoostPluginLoader.cpp
  src/Cli.cpp
  src/PluginCollector.cpp
  src/PluginSystem.cpp

  ${CPPPS_LOGGING_SOURCES}
  )
add_library(cppps::cppps ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
  PUBLIC
  include
  ${CMAKE_CURRENT_BINARY_DIR}/include
  ${CPPPS_LOGGING_INCLUDE_DIR}

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
  ${CPPPS_LOGGING_LIBS}
  )

enable_testing(ON)

option(CPPPS_UNIT_TESTS_BUILD "Build CPPPS unit tests" ON)
if (${CPPPS_UNIT_TESTS_BUILD})
  add_subdirectory(tests/unit)
endif()

option(CPPPS_INTEGRATION_TESTS_BUILD "Build CPPPS integraton tests" ON)
if (${CPPPS_INTEGRATION_TESTS_BUILD})
  add_subdirectory(tests/integration)
endif()

