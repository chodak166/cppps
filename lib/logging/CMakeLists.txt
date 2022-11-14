cmake_minimum_required(VERSION 3.16)

set(LIB_BASE_NAME logging)
project(cppps-${LIB_BASE_NAME} VERSION 0.1.1)

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

option(CPPPS_LOGGING_USE_ELPP "Use EasyLogging++" ON)

if (${CPPPS_LOGGING_USE_ELPP})
  find_package(ELPP REQUIRED)
  set(CPPPS_LOGGING_HEADER "\"easylogging++.h\"")
  set(SOURCES ${ELPP_SOURCES})
  set(EXTRA_LIBRARIES ${ELPP_LIBRARIES})
  set(EXTRA_INCLUDE_DIRS ${ELPP_INCLUDE_DIR})
  set(EXTRA_PUBLIC_HEADERS "${ELPP_INCLUDE_DIR}/easylogging++.h")
else()
  set(CPPPS_LOGGING_HEADER "<cppps/logging/stdeasylog.h>")
  set(SOURCES src/StdLogging.cpp)
endif()

set(CPPPS_LOGGING_GEN_INCLUDES ${CMAKE_CURRENT_BINARY_DIR}/include/)
configure_file(src/Logging.h.in ${CPPPS_LOGGING_GEN_INCLUDES}/cppps/logging/Logging.h)

add_library(${TARGET_OBJ} OBJECT ${SOURCES})

set_target_properties(${TARGET_OBJ} PROPERTIES
  ENABLE_EXPORTS ON
  POSITION_INDEPENDENT_CODE ON
  )

target_include_directories(${TARGET_OBJ}
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
    ${CPPPS_LOGGING_GEN_INCLUDES}
    ${EXTRA_INCLUDE_DIRS}

  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/
  )

target_link_libraries(${TARGET_OBJ}
  pthread
  ${EXTRA_LIBRARIES}
  )

set(TARGET_STATIC "${PROJECT_NAME}-static")
add_library(${TARGET_STATIC} STATIC $<TARGET_OBJECTS:${TARGET_OBJ}>)
set_target_properties(${TARGET_STATIC} PROPERTIES
  EXPORT_NAME ${LIB_BASE_NAME}
  OUTPUT_NAME ${PROJECT_NAME}
  )

target_include_directories(${TARGET_STATIC}
  INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
    $<BUILD_INTERFACE:${CPPPS_LOGGING_GEN_INCLUDES}>
    $<BUILD_INTERFACE:${EXTRA_INCLUDE_DIRS}>
  )

set(TARGET_SHARED "${PROJECT_NAME}-shared")
add_library(${TARGET_SHARED} SHARED $<TARGET_OBJECTS:${TARGET_OBJ}>)
set_target_properties(${TARGET_SHARED} PROPERTIES
  EXPORT_NAME ${LIB_BASE_NAME}-shared
  OUTPUT_NAME ${PROJECT_NAME}
  VERSION ${PROJECT_VERSION}
  SOVERSION ${PROJECT_VERSION_MAJOR}
  )

target_include_directories(${TARGET_SHARED}
  INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
    $<BUILD_INTERFACE:${CPPPS_LOGGING_GEN_INCLUDES}>
    $<BUILD_INTERFACE:${EXTRA_INCLUDE_DIRS}>
  )

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

# --- install ---

include(GNUInstallDirs)

file(GLOB DEV_HEADERS "include/cppps/logging/*.h")

install(
  TARGETS ${TARGET_STATIC}
  EXPORT ${PROJECT_NAME}-targets
  DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT bin
  )

install(
  TARGETS ${TARGET_SHARED}
  EXPORT ${PROJECT_NAME}-targets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT bin
  NAMELINK_SKIP
  )

install(
  TARGETS ${TARGET_SHARED}
  EXPORT ${PROJECT_NAME}-targets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT dev
  NAMELINK_ONLY
  )

install(
  FILES ${DEV_HEADERS}
  ${CPPPS_LOGGING_GEN_INCLUDES}/cppps/logging/Logging.h
  ${EXTRA_PUBLIC_HEADERS}
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/cppps/logging COMPONENT dev
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

# --- subdirectories ---

common_option_subdir(CPPPS_LOGGING_BUILD_DL_PLUGIN
 "Build logging DL plugin (shared logger)"
 "${CMAKE_CURRENT_LIST_DIR}/dl-plugin")
