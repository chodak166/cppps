cmake_minimum_required(VERSION 3.5)

project(cppps-dl LANGUAGES CXX)

if(TARGET ${PROJECT_NAME})
  message("Target ${PROJECT_NAME} already added, skipping")
  return()
endif()


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/lib)

get_filename_component(PROJECT_ROOT ../../ ABSOLUTE)

include("${PROJECT_ROOT}/cmake/helpers.cmake")
list(APPEND CMAKE_MODULE_PATH "${PROJECT_ROOT}/cmake/modules")

find_package(CLI MODULE)

option(CPPPS_DL_USE_BOOST "Use Boost DLL" OFF)
if (${CPPPS_DL_USE_BOOST})
  find_package(Boost REQUIRED COMPONENTS filesystem system)
  add_definitions(-DCPPPS_DL_USE_BOOST)
  set(SOURCES src/BoostPluginLoader.cpp)
  set(LIBRARIES dl ${Boost_LIBRARIES})
else()
  if(WIN32)
    set(SOURCES src/WindowsPluginLoader.cpp)
  else()
    set(SOURCES src/DlopenPluginLoader.cpp)
    set(LIBRARIES dl stdc++fs)
  endif()
endif()

set(SOURCES ${SOURCES}
  src/Application.cpp
  src/Cli.cpp
  src/PluginCollector.cpp
  src/PluginSystem.cpp
  src/OsUtils.cpp
  )

add_library(${PROJECT_NAME} ${SOURCES})
add_library(cppps::dl ALIAS ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} ${LIBRARIES})

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


# --- tests ---

enable_testing(ON)

set(LIB_ROOT ${CMAKE_CURRENT_LIST_DIR})

set(CPPPS_DL_UNIT_TESTS_BUILD OFF CACHE BOOL "Build CPPPS unit tests")
if (${CPPPS_DL_UNIT_TESTS_BUILD})
  add_subdirectory(tests/unit)
endif()

set(CPPPS_DL_INTEGRATION_TESTS_BUILD OFF CACHE BOOL "Build CPPPS integraton tests")
if (${CPPPS_DL_INTEGRATION_TESTS_BUILD})
  add_subdirectory(tests/integration)
endif()

