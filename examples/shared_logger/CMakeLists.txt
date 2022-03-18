cmake_minimum_required(VERSION 3.5)

project(cppps-shared-logger VERSION 1.0.0 DESCRIPTION "CPPPS shared logger example")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/lib)
set(PLUGIN_DIR_NAME "logex_plugins")

set(CPPPS_LOGGING_BUILD_DL_PLUGIN ON)
set(CPPPS_LOGGING_DL_PLUGIN_OUTPUT_DIR ${PLUGIN_DIR_NAME})
add_subdirectory("../../" cppps)

configure_file(Version.h.in
  ${CMAKE_BINARY_DIR}/${PROJECT_NAME}/Version.h)

add_executable(${PROJECT_NAME}
 Main.cpp
 )

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_BINARY_DIR})


target_link_libraries(${PROJECT_NAME}
  cppps::dl
  cppps::logging
 )

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${PLUGIN_DIR_NAME})

add_subdirectory(LogProviderPlugin)
add_subdirectory(LogConsumerPlugin)
