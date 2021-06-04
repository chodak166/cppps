find_package(Catch2 MODULE)
find_package(FakeIt MODULE)

include_directories(
  ${LIB_ROOT}/include
  ${LIB_ROOT}/submodules
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
  ${LIB_ROOT}/src/PluginSystem.cpp
  )
