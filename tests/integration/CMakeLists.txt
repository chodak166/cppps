include_directories(
  ${PROJECT_ROOT}/include
)

add_subdirectory(${PROJECT_ROOT}/submodules/catch2 catch2)
add_subdirectory(test_plugins)

set(COMMON_LIBS
    Catch2::Catch2
  )

add_test_executable(TARGET plugin-loader-test
  SOURCES
  BoostPluginLoader.test.cpp
  main.cpp

  LIBS
  ${COMMON_LIBS}
  Boost::system
  Boost::filesystem
  dl
  )

add_test_executable(TARGET plugin-collector-test
  SOURCES
  PluginCollector.test.cpp

  LIBS
  ${COMMON_LIBS}
  )
