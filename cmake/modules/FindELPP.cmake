
set(EASYLOGGINGPP_DIR "${PROJECT_SOURCE_DIR}/submodules/easyloggingpp" CACHE STRING "Easylogging++ root directory")

set(EASYLOGGINGPP_PATHS
  ${EASYLOGGINGPP_DIR}
  $ENV{EASYLOGGINGPP_DIR}
  "${EXTERN_DIR}/easyloggingpp"
  "${PROJECT_SOURCE_DIR}/extern/easyloggingpp"
  "${PROJECT_SOURCE_DIR}/external/easyloggingpp"
  "${PROJECT_SOURCE_DIR}/submodules/easyloggingpp"
  "${CMAKE_SOURCE_DIR}/extern/easyloggingpp"
  "${CMAKE_SOURCE_DIR}/external/easyloggingpp"
  "${CMAKE_SOURCE_DIR}/submodules/easyloggingpp"
  "${CPPPS_ROOT}/submodules/easyloggingpp"
  )

find_path(ELPP_INCLUDE_DIR
  easylogging++.h
  PATH_SUFFIXES include src
  PATHS ${EASYLOGGINGPP_PATHS}
  )

find_library(EASYLOGGINGPP_LIBRARIES
  NAMES libeasyloggingpp.a libeasyloggingpp.dylib libeasyloggingpp
  HINTS "${CMAKE_PREFIX_PATH}/lib"
  )

if (NOT EASYLOGGINGPP_LIBRARIES)
  find_path(ELPP_SRC_DIR
    easylogging++.cc
    PATH_SUFFIXES src
    PATHS ${EASYLOGGINGPP_PATHS}
    )

  if (NOT ELPP_SRC_DIR)
    message(FATAL_ERROR "The easylogging++ library has not been found. Consider using EASYLOGGINGPP_DIR variable." )
  else()
    set(ELPP_SOURCES
      ${ELPP_SRC_DIR}/easylogging++.cc
      ${PROJECT_SOURCE_DIR}/src/ElppLogging.cpp)
  endif()

endif()

add_definitions(
  -D_ELPP
  -DELPP_NO_DEFAULT_LOG_FILE
  )

find_package_handle_standard_args(ELPP REQUIRED_VARS ELPP_INCLUDE_DIR)
