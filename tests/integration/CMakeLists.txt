set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost REQUIRED COMPONENTS filesystem system)

include_directories(
  ${PROJECT_ROOT}/include
  )

if (NOT TARGET Catch2)
  add_subdirectory(${PROJECT_ROOT}/submodules/catch2 catch2)
endif()

if (NOT TARGET CLI11)
  add_subdirectory(${PROJECT_ROOT}/submodules/cli11 cli11)
endif()

add_subdirectory(dummy_plugins)
add_subdirectory(app_plugins)

set(COMMON_LIBS
  cppps
  stdc++fs
  Catch2::Catch2
  CLI11::CLI11
  )

add_test_executable(TARGET plugin-collector-test
  SOURCES
  PluginCollector.test.cpp

  LIBS
  ${COMMON_LIBS}
  )

add_test_executable(TARGET plugin-loader-test
  SOURCES
  BoostPluginLoader.test.cpp

  LIBS
  ${COMMON_LIBS}
  Boost::system
  Boost::filesystem
  dl
  )

add_test_executable(TARGET application-test
  SOURCES
  Application.test.cpp

  LIBS
  ${COMMON_LIBS}
  Boost::system
  Boost::filesystem
  dl
  )

# export symbols to enable shared template instantiation (including resources)
set_target_properties(application-test PROPERTIES ENABLE_EXPORTS ON)
