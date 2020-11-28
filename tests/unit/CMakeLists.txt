include_directories(
  ${PROJECT_ROOT}/include
)

if (NOT TARGET Catch2)
  add_subdirectory(${PROJECT_ROOT}/submodules/catch2 catch2)
endif()

set(COMMON_LIBS
    Catch2::Catch2
  )

add_test_executable(TARGET plugin-system-test
  SOURCES
  PluginSystem.test.cpp

  LIBS
  ${COMMON_LIBS}
  )

add_test_executable(TARGET graph-dependency-resolver-test
  SOURCES
  GraphDependencyResolver.test.cpp

  LIBS
  ${COMMON_LIBS}
  )
