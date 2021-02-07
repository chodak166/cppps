set(CPPPS_USE_ELPP OFF CACHE BOOL "Use Easylogging++ logging library")

if (CPPPS_USE_ELPP)
  add_definitions(-DCPPPS_USE_ELPP)

  set(EASYLOGGINGPP_DIR "${PROJECT_SOURCE_DIR}/submodules/easyloggingpp/src" CACHE STRING "Easylogging++ root directory")

  set(EASYLOGGINGPP_PATHS
    ${EASYLOGGINGPP_DIR}
    $ENV{EASYLOGGINGPP_DIR}
    )

  find_path(LOGGING_INCLUDE_DIR
    easylogging++.h
    PATH_SUFFIXES include src
    PATHS ${EASYLOGGINGPP_PATHS}
    )

  if (LOGGING_INCLUDE_DIR)
    set(LOGGING_HEADER "<easylogging++.h>")
  endif()

  find_library(EASYLOGGINGPP_LIBRARY
    NAMES libeasyloggingpp.a libeasyloggingpp.dylib libeasyloggingpp
    HINTS "${CMAKE_PREFIX_PATH}/lib"
    )

  if (NOT EASYLOGGINGPP_LIBRARY)
    find_path(LOGGING_SRC_DIR
      easylogging++.cc
      PATH_SUFFIXES src
      PATHS ${EASYLOGGINGPP_PATHS}
      )

    if (NOT LOGGING_SRC_DIR)
      message(FATAL_ERROR "CPPPS_USE_ELPP is set but the easylogging++ library has not been found. Consider using EASYLOGGINGPP_DIR variable." )
    else()
      set(LOGGING_SOURCES
        ${LOGGING_SRC_DIR}/easylogging++.cc
        ${PROJECT_SOURCE_DIR}/src/ElppLogging.cpp)
    endif()
  else()
    set(LOGGING_LIBS ${EASYLOGGINGPP_LIBRARY})
  endif()

else()
  set(LOGGING_SOURCES ${PROJECT_SOURCE_DIR}/src/StdLogging.cpp)
  set(LOGGING_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/src)
  set(LOGGING_HEADER "\"cppps/stdeasylog.h\"")
endif()

find_package_handle_standard_args(Logging REQUIRED_VARS LOGGING_INCLUDE_DIR LOGGING_HEADER)
