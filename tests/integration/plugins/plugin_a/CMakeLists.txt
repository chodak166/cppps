cmake_minimum_required(VERSION 3.5)

project(plugin_a LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/plugins)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost REQUIRED COMPONENTS filesystem system)

add_library(${PROJECT_NAME} MODULE
  main.cpp
  Plugin.cpp
  )

set_target_properties(${PROJECT_NAME} PROPERTIES
  CXX_VISIBILITY_PRESET hidden)

target_include_directories(${PROJECT_NAME}
  PRIVATE
  ${PROJECT_ROOT}/include
  )

target_link_libraries(${PROJECT_NAME}
  ${Boost_LIBRARIES}
  )

