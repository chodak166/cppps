cmake_minimum_required(VERSION 3.5)

project(test_plugins LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/test_plugins)

macro(add_plugin PLUGIN_NAME PLUGIN_SRC)

  add_library(${PLUGIN_NAME} MODULE
    ${PLUGIN_SRC}
    )

  set_target_properties(${PLUGIN_NAME} PROPERTIES
    CXX_VISIBILITY_PRESET hidden)

  target_include_directories(${PLUGIN_NAME}
    PRIVATE
    ${LIB_ROOT}/include
    )

  target_compile_definitions(${PLUGIN_NAME} PRIVATE
    -DPLUGIN_NAME="${PLUGIN_NAME}"
    -DPLUGIN_VERSION="1.0.0"
    )

endmacro()


add_plugin(plugin_a   PluginA.cpp)
add_plugin(plugin_b   PluginB.cpp)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/test_plugins_bad)
add_plugin(plugin_bad PluginBad.cpp)
