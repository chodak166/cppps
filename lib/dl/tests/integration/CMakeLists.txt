find_package(Catch2 MODULE)
find_package(FakeIt MODULE)

include_directories(
  ${CPPPS_CATCH2_INCLUDE_DIR}
  ${CPPPS_FAKEIT_INCLUDE_DIR}
)

add_subdirectory(dummy_plugins)
add_subdirectory(app_plugins)

set(COMMON_LIBS
  cppps::dl
  )

add_test_executable(TARGET plugin-collector-test
  SOURCES
  PluginCollector.test.cpp

  LIBS
  ${COMMON_LIBS}
  )

add_test_executable(TARGET boost-plugin-loader-test
  SOURCES
  BoostPluginLoader.test.cpp

  LIBS
  ${COMMON_LIBS}
  )

add_test_executable(TARGET application-test
  SOURCES
  Application.test.cpp

  LIBS
  ${COMMON_LIBS}
  )

# export symbols to enable shared template instantiation (including resources)
set_target_properties(application-test PROPERTIES ENABLE_EXPORTS ON)
