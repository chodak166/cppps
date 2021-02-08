set(CPPPS_CATCH2_DIR "${PROJECT_ROOT}/submodules/catch2" CACHE STRING "Catch2 root directory")

include_directories(
#  ${PROJECT_ROOT}/include
#  ${PROJECT_ROOT}/submodules
  ${CPPPS_CATCH2_DIR}/include
  ${CPPPS_CATCH2_DIR}/single_include
)

add_subdirectory(dummy_plugins)
add_subdirectory(app_plugins)

set(COMMON_LIBS
  cppps::cppps
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
  )

add_test_executable(TARGET application-test
  SOURCES
  Application.test.cpp

  LIBS
  ${COMMON_LIBS}
  )

# export symbols to enable shared template instantiation (including resources)
set_target_properties(application-test PROPERTIES ENABLE_EXPORTS ON)
