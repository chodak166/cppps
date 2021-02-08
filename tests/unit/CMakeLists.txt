set(CPPPS_CATCH2_DIR "${PROJECT_ROOT}/submodules/catch2" CACHE STRING "Catch2 root directory")

include_directories(
  ${PROJECT_ROOT}/include
  ${PROJECT_ROOT}/submodules
  ${CPPPS_CATCH2_DIR}/include
  ${CPPPS_CATCH2_DIR}/single_include
)

add_test_executable(TARGET digraph-test
  SOURCES
  Digraph.test.cpp
  )

add_test_executable(TARGET plugin-system-test
  SOURCES
  PluginSystem.test.cpp
  ${PROJECT_ROOT}/src/PluginSystem.cpp
  )
