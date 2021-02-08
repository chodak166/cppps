set(CPPPS_USE_ELPP OFF CACHE BOOL "Use Easylogging++ logging library")

if (CPPPS_USE_ELPP)
  add_definitions(
    -DCPPPS_USE_ELPP
    -DELPP_NO_DEFAULT_LOG_FILE
    )

  set(EASYLOGGINGPP_DIR "${PROJECT_SOURCE_DIR}/submodules/easyloggingpp" CACHE STRING "Easylogging++ root directory")

  set(EASYLOGGINGPP_PATHS
    ${EASYLOGGINGPP_DIR}
    $ENV{EASYLOGGINGPP_DIR}
    "${PROJECT_SOURCE_DIR}/extern/easyloggingpp"
    "${PROJECT_SOURCE_DIR}/external/easyloggingpp"
    )

  find_path(CPPPS_LOGGING_INCLUDE_DIR
    easylogging++.h
    PATH_SUFFIXES include src
    PATHS ${EASYLOGGINGPP_PATHS}
    )

  if (CPPPS_LOGGING_INCLUDE_DIR)
    set(CPPPS_LOGGING_HEADER "<easylogging++.h>")
  endif()

  find_library(EASYLOGGINGPP_LIBRARY
    NAMES libeasyloggingpp.a libeasyloggingpp.dylib libeasyloggingpp
    HINTS "${CMAKE_PREFIX_PATH}/lib"
    )

  if (NOT EASYLOGGINGPP_LIBRARY)
    find_path(CPPPS_LOGGING_SRC_DIR
      easylogging++.cc
      PATH_SUFFIXES src
      PATHS ${EASYLOGGINGPP_PATHS}
      )

    if (NOT CPPPS_LOGGING_SRC_DIR)
      message(FATAL_ERROR "CPPPS_USE_ELPP is set but the easylogging++ library has not been found. Consider using EASYLOGGINGPP_DIR variable." )
    else()
      set(CPPPS_LOGGING_SOURCES
        ${LOGGING_SRC_DIR}/easylogging++.cc
        ${PROJECT_SOURCE_DIR}/src/ElppLogging.cpp)
    endif()
  else()
    set(CPPPS_LOGGING_LIBS ${EASYLOGGINGPP_LIBRARY})
  endif()

else()
  set(CPPPS_LOGGING_SOURCES ${PROJECT_SOURCE_DIR}/src/StdLogging.cpp)
  set(CPPPS_LOGGING_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/src)
  set(CPPPS_LOGGING_HEADER "\"cppps/stdeasylog.h\"")
endif()

find_package_handle_standard_args(Logging REQUIRED_VARS CPPPS_LOGGING_INCLUDE_DIR CPPPS_LOGGING_HEADER)
