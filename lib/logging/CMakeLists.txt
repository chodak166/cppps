cmake_minimum_required(VERSION 3.5)

project(cppps-logging VERSION 1.0.0)

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

option(CPPPS_LOGGING_USE_ELPP "Use EasyLogging++" ON)

if (${CPPPS_LOGGING_USE_ELPP})
  find_package(ELPP REQUIRED)
  set(CPPPS_LOGGING_HEADER "<easylogging++.h>")
  set(SOURCES ${ELPP_SOURCES})
  set(EXTRA_LIBRARIES ${ELPP_LIBRARIES})
  set(EXTRA_INCLUDE_DIRS ${ELPP_INCLUDE_DIR})
else()
  set(CPPPS_LOGGING_HEADER "<cppps/logging/stdeasylog.h>")
  set(SOURCES src/StdLogging.cpp)
endif()

configure_file(src/Logging.h.in ${CMAKE_CURRENT_BINARY_DIR}/include/cppps/logging/Logging.h)



add_library("${PROJECT_NAME}-obj" OBJECT ${SOURCES})

set_target_properties("${PROJECT_NAME}-obj" PROPERTIES
  ENABLE_EXPORTS ON
  POSITION_INDEPENDENT_CODE ON
  )

target_include_directories("${PROJECT_NAME}-obj"
  PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/include
  ${CMAKE_CURRENT_BINARY_DIR}/include/
  ${EXTRA_INCLUDE_DIRS}

  PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/src/
  )

target_link_libraries("${PROJECT_NAME}-obj"
  pthread
  ${EXTRA_LIBRARIES}
  )

set(TARGET_STATIC "${PROJECT_NAME}-static")
add_library(${TARGET_STATIC} STATIC $<TARGET_OBJECTS:${PROJECT_NAME}-obj>)
add_library(cppps::logging ALIAS ${TARGET_STATIC})
target_link_libraries(${TARGET_STATIC} PUBLIC ${PROJECT_NAME}-obj)

set(TARGET_SHARED ${PROJECT_NAME})
add_library(${TARGET_SHARED} SHARED $<TARGET_OBJECTS:${PROJECT_NAME}-obj>)
add_library(cppps::logging-shared ALIAS ${TARGET_SHARED})
target_link_libraries(${TARGET_SHARED} PUBLIC ${PROJECT_NAME}-obj)

# --- install ---

include(GNUInstallDirs)

file(GLOB DEV_HEADERS "include/cppps/logging/*.h")

install(
  TARGETS ${TARGET_SHARED} ${TARGET_STATIC}
  DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT bin
  )

install(
  FILES ${DEV_HEADERS}
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/cppps/logging COMPONENT dev
  )


# --- subdirectories ---

common_option_subdir(CPPPS_LOGGING_BUILD_DL_PLUGIN
 "Build logging DL plugin (shared logger)"
 "${CMAKE_CURRENT_LIST_DIR}/dl-plugin")
