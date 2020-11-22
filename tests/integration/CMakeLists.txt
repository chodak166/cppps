include_directories(
  ${PROJECT_ROOT}/include
)

add_subdirectory(${PROJECT_ROOT}/submodules/catch2 catch2)
add_subdirectory(plugins/plugin_a)

add_test_executable(TARGET plugin-loader-test
  SOURCES
  PluginLoader.test.cpp
  main.cpp

  LIBS
  Catch2::Catch2
  Boost::system
  Boost::filesystem
  dl
  )
