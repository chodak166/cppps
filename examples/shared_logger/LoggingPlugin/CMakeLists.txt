cmake_minimum_required(VERSION 3.5)

project(LoggingPlugin LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_library(${PROJECT_NAME} MODULE
  Plugin.cpp
  )

set_target_properties(${PROJECT_NAME} PROPERTIES
  CXX_VISIBILITY_PRESET hidden)

target_link_libraries(${PROJECT_NAME}
  cppps::dl
  cppps::logging
  )

target_compile_definitions(${PROJECT_NAME} PRIVATE
  -DPLUGIN_NAME="${PROJECT_NAME}"
  -DPLUGIN_VERSION="1.0.0"
  )
