cmake_minimum_required(VERSION 3.5)

project(cppps-minimal VERSION 1.0.0 DESCRIPTION "CPPPS minimal example")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/lib)

set(CPPPS_DIR "../../" CACHE STRING "CPPPS library root directory")

add_subdirectory(${CPPPS_DIR} cppps)

configure_file(version.h.in
  ${PROJECT_SOURCE_DIR}/version.h)

add_executable(${PROJECT_NAME}
 main.cpp
 )

target_link_libraries(${PROJECT_NAME}
 cppps
 )

add_subdirectory(ProviderPlugin)
add_subdirectory(ConsumerPlugin)
