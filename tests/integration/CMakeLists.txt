include_directories(
  ${PROJECT_ROOT}/include
)

if (NOT TARGET Catch2)
  add_subdirectory(${PROJECT_ROOT}/submodules/catch2 catch2)
endif()

add_subdirectory(test_plugins)

set(COMMON_LIBS
    Catch2::Catch2
  )

add_test_executable(TARGET plugin-loader-test
  SOURCES
  BoostPluginLoader.test.cpp
  main.cpp
  ${PROJECT_ROOT}/src/BoostPluginLoader.cpp

  LIBS
  ${COMMON_LIBS}
  Boost::system
  Boost::filesystem
  dl
  )

add_test_executable(TARGET plugin-collector-test
  SOURCES
  PluginCollector.test.cpp
  ${PROJECT_ROOT}/src/PluginCollector.cpp

  LIBS
  ${COMMON_LIBS}
  )
