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

add_subdirectory(dummy_plugins)
add_subdirectory(app_plugins)

set(COMMON_LIBS
    stdc++fs
    Catch2::Catch2
  )

add_test_executable(TARGET plugin-collector-test
  SOURCES
  PluginCollector.test.cpp
  ${PROJECT_ROOT}/src/PluginCollector.cpp

  LIBS
  ${COMMON_LIBS}
  )

add_test_executable(TARGET plugin-loader-test
  SOURCES
  BoostPluginLoader.test.cpp
  ${PROJECT_ROOT}/src/BoostPluginLoader.cpp

  LIBS
  ${COMMON_LIBS}
  Boost::system
  Boost::filesystem
  dl
  )

add_test_executable(TARGET application-test
  SOURCES
  Application.test.cpp
  ${PROJECT_ROOT}/src/BoostPluginLoader.cpp
  ${PROJECT_ROOT}/src/PluginCollector.cpp
  ${PROJECT_ROOT}/src/PluginSystem.cpp

  LIBS
  ${COMMON_LIBS}
  Boost::system
  Boost::filesystem
  dl
  )

# export symbols to enable shared template instantiation (including resources)
set_target_properties(application-test PROPERTIES ENABLE_EXPORTS ON)
