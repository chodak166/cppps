

# Function adding unit/integration test target with the CTest "add_test" call.
#
# Usage:
#
# add_test_executable([SKIPMAIN] TARGET <target>
#                     SOURCES <additional sources>
#                     [LIBS <libraries>])
#
# The target given after "TARGET" keyword requires <target.cpp> test file.
# This function will look for the main.cpp file as test entrypoint
# unles the "SKIPMAIN" flag is provided.
#
# Example 1
# - adding footest.cpp test testing ../src/foo.cpp unit:
# add_test_executable(TARGET footest SOURCES ../src/foo.cpp)
#
# Example 2
# - adding mootest.cpp containing main function, test testing ../src/moo.cpp unit
# and linking pthread and stdc++fs libraries:
# add_test_executable(SKIPMAIN TARGET mootest SOURCES ../src/moo.cpp LIBS pthread stdc++fs)

function(add_test_executable)
  cmake_parse_arguments(TEST_EXEC SKIPMAIN TARGET "SOURCES;LIBS" ${ARGN})

  if (NOT ${TEST_EXEC_SKIPMAIN} AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")
    set(TEST_EXEC_SOURCES
      ${TEST_EXEC_SOURCES}
      "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
      )
  endif()

  if (EXISTS "${TEST_EXEC_TARGET}.cpp")
    set(TEST_EXEC_SOURCES ${TEST_EXEC_SOURCES} ${TEST_EXEC_TARGET}.cpp)
  endif()

  if (EXISTS "main.cpp")
    set(TEST_EXEC_SOURCES ${TEST_EXEC_SOURCES} main.cpp)
  endif()

  add_executable(${TEST_EXEC_TARGET} ${TEST_EXEC_SOURCES})

  if (DEFINED TEST_EXEC_LIBS)
    target_link_libraries(${TEST_EXEC_TARGET} PRIVATE ${TEST_EXEC_LIBS})
  endif()

  target_compile_options(${TEST_EXEC_TARGET} PRIVATE "-O0" )
  add_test(NAME ${TEST_EXEC_TARGET} COMMAND ${TEST_EXEC_TARGET})


endfunction()


# Function adding an option with initial value determined by the project type.
#
# The intended result is to avoid compiling additional targets (such as tests
# and examples) when a library has been added as a subdirectory. The default
# value will be set to "ON" only if the project is top-level project or it
# is subproject of the "common-cpp" project.
#
# Usage (same as "option"):
# common_option(<cache variable name> <description>)

function(common_option VAR DESCRIPTION)
  set(OPTION_INIT_VALUE OFF)
  if (${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME}
      OR ${CMAKE_PROJECT_NAME} STREQUAL "common-cpp")
    set(OPTION_INIT_VALUE ON)
  endif()

  option(${VAR} "${DESCRIPTION}" ${OPTION_INIT_VALUE})
endfunction()


# Helper function adding given subdirectory if option value is set to "ON".
#
# Usage:
# common_option_subdir(<cache variable name> <description> <subdirectory>)

function(common_option_subdir VAR DESCRIPTION SUBDIR)
  common_option(${VAR} "${DESCRIPTION}")

  if(${VAR})
    add_subdirectory("${SUBDIR}")
  endif()
endfunction()

# Macro assuring that given target does not yet exist in the global namespace.
#
# Usage:
# _ASSERT_UNIQUE(<target name>)

macro(_ASSERT_UNIQUE TARGET)
  if (TARGET ${TARGET})
    message(STATUS "The target ${TARGET} is already present in ${CMAKE_PROJECT_NAME}, skipping")
    return()
  endif()
endmacro()
