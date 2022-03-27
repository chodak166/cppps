cmake_minimum_required(VERSION 3.5)

project(cppps-dl LANGUAGES CXX VERSION 0.1.0)

set(TARGET_OBJ "${PROJECT_NAME}-obj")
if(TARGET ${TARGET_OBJ})
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

add_library(${TARGET_OBJ} OBJECT ${SOURCES})

set_target_properties(${TARGET_OBJ} PROPERTIES
  ENABLE_EXPORTS ON
  POSITION_INDEPENDENT_CODE ON
  )

target_link_libraries(${TARGET_OBJ} ${LIBRARIES})

target_include_directories(${TARGET_OBJ}
  PUBLIC
    include
    ${CMAKE_CURRENT_BINARY_DIR}/include
  PRIVATE
    src
    ${CPPPS_CLI_INCLUDE_DIR}
  )

set(TARGET_STATIC "${PROJECT_NAME}-static")
add_library(${TARGET_STATIC} STATIC $<TARGET_OBJECTS:${TARGET_OBJ}>)
set_target_properties(${TARGET_STATIC} PROPERTIES EXPORT_NAME dl)

set(TARGET_SHARED "${PROJECT_NAME}-shared")
add_library(${TARGET_SHARED} SHARED $<TARGET_OBJECTS:${TARGET_OBJ}>)
set_target_properties(${TARGET_SHARED} PROPERTIES EXPORT_NAME dl-shared)

include(CMakePackageConfigHelpers)
configure_package_config_file(
  ${PROJECT_NAME}-config.cmake.in
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake"
  INSTALL_DESTINATION "${CMAKE_INSTALL_DATADIR}/cppps/cmake"
  )

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion
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


# --- install ---

include(GNUInstallDirs)

file(GLOB DEV_HEADERS "include/cppps/dl/*.h")

install(
  TARGETS ${TARGET_SHARED} ${TARGET_STATIC}
  EXPORT ${PROJECT_NAME}-targets
  DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT bin
  )

install(
  FILES ${DEV_HEADERS}
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/cppps/dl COMPONENT dev
  )

install(
    EXPORT ${PROJECT_NAME}-targets
    FILE ${PROJECT_NAME}-targets.cmake
    DESTINATION "${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}/cmake" COMPONENT dev
    NAMESPACE cppps::
)

install(
    FILES
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake"
    DESTINATION "${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}/cmake" COMPONENT dev
)
