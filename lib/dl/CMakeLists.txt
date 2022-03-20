cmake_minimum_required(VERSION 3.5)

project(cppps-dl LANGUAGES CXX VERSION 0.1.0)

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

include("${PROJECT_ROOT}/cmake/include/helpers.cmake")
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

add_library("${PROJECT_NAME}-obj" OBJECT ${SOURCES})

set_target_properties("${PROJECT_NAME}-obj" PROPERTIES
  ENABLE_EXPORTS ON
  POSITION_INDEPENDENT_CODE ON
  )

target_link_libraries("${PROJECT_NAME}-obj" ${LIBRARIES})

target_include_directories("${PROJECT_NAME}-obj"
  PUBLIC
  include
  ${CMAKE_CURRENT_BINARY_DIR}/include

  PRIVATE
  src
  ${CPPPS_CLI_INCLUDE_DIR}
  )

set(TARGET_STATIC "${PROJECT_NAME}-static")
add_library(${TARGET_STATIC} STATIC $<TARGET_OBJECTS:${PROJECT_NAME}-obj>)
add_library(cppps::dl ALIAS ${TARGET_STATIC})
target_link_libraries(${TARGET_STATIC} PUBLIC ${PROJECT_NAME}-obj)

set(TARGET_SHARED ${PROJECT_NAME})
add_library(${TARGET_SHARED} SHARED $<TARGET_OBJECTS:${PROJECT_NAME}-obj>)
add_library(cppps::dl-shared ALIAS ${TARGET_SHARED})
target_link_libraries(${TARGET_SHARED} PUBLIC ${PROJECT_NAME}-obj)


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


# --- install ---

include(GNUInstallDirs)

file(GLOB DEV_HEADERS "include/cppps/dl/*.h")

install(
  TARGETS ${TARGET_SHARED} ${TARGET_STATIC}
  DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT bin
  )

install(
  FILES ${DEV_HEADERS}
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/cppps/dl COMPONENT dev
  )
