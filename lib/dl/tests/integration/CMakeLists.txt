find_package(Catch2 MODULE)
find_package(FakeIt MODULE)

find_package(CPPPS-DL MODULE REQUIRED)

include_directories(
  ${LIB_ROOT}/src
  ${CPPPS_CATCH2_INCLUDE_DIR}
  ${CPPPS_FAKEIT_INCLUDE_DIR}
)

add_subdirectory(test_plugins)

set(COMMON_LIBS
  cppps::dl
  )

add_test_executable(TARGET plugin-collector-test
  SOURCES
  PluginCollector.test.cpp

  LIBS
  ${COMMON_LIBS}
  )

if (${CPPPS_DL_USE_BOOST})
  add_test_executable(TARGET boost-plugin-loader-test
    SOURCES
    BoostPluginLoader.test.cpp

    LIBS
    ${COMMON_LIBS}
    )
else()
  if (WIN32)
    add_test_executable(TARGET windows-plugin-loader-test
      SOURCES
      WindowsPluginLoader.test.cpp

      LIBS
      ${COMMON_LIBS}
      )
  else()
    add_test_executable(TARGET dlopen-plugin-loader-test
      SOURCES
      DlopenPluginLoader.test.cpp

      LIBS
      ${COMMON_LIBS}
      )
  endif()
endif()

add_test_executable(TARGET application-test
  SOURCES
  Application.test.cpp

  LIBS
  ${COMMON_LIBS}
  )

# export symbols to enable shared template instantiation (including resources)
set_target_properties(application-test PROPERTIES ENABLE_EXPORTS ON)
