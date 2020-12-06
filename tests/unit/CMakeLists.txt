include_directories(
  ${PROJECT_ROOT}/include
  ${PROJECT_ROOT}/submodules
)

if (NOT TARGET Catch2)
  add_subdirectory(${PROJECT_ROOT}/submodules/catch2 catch2)
endif()

set(COMMON_LIBS
    Catch2::Catch2
  )

add_test_executable(TARGET digraph-test
  SOURCES
  Digraph.test.cpp

  LIBS
  ${COMMON_LIBS}
  )

add_test_executable(TARGET plugin-system-test
  SOURCES
  PluginSystem.test.cpp

  LIBS
  ${COMMON_LIBS}
  )
