find_package(Catch2 MODULE)
find_package(FakeIt MODULE)

include_directories(
  ${PROJECT_ROOT}/include
  ${PROJECT_ROOT}/submodules
  ${CPPPS_CATCH2_INCLUDE_DIR}
  ${CPPPS_FAKEIT_INCLUDE_DIR}
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
