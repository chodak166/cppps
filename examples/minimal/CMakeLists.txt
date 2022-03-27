cmake_minimum_required(VERSION 3.5)

project(cppps-minimal VERSION 1.0.0 DESCRIPTION "CPPPS minimal example")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/lib)
set(PLUGIN_DIR_NAME "minex_plugins")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../../cmake/modules")

find_package(CPPPS-DL MODULE REQUIRED)

configure_file(Version.h.in
  ${CMAKE_BINARY_DIR}/${PROJECT_NAME}/Version.h)

add_executable(${PROJECT_NAME}
 Main.cpp
 )

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_BINARY_DIR})

target_link_libraries(${PROJECT_NAME}
  cppps::dl
 )

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${PLUGIN_DIR_NAME})

add_subdirectory(ProviderPlugin)
add_subdirectory(ConsumerPlugin)
