macro(CPPPS_ADD_MODULE CPPPS_LIB_DIR CPPPS_LIB_TARGET CPPPS_LIB_ALIAS)

  if (NOT CPPPS_ROOT)
    get_filename_component(CPPPS_ROOT ${CMAKE_CURRENT_LIST_DIR}/../.. ABSOLUTE)
  endif()

  set(CPPPS_LIB_ABS_DIR "${CPPPS_ROOT}/lib/${CPPPS_LIB_DIR}")

  if (NOT TARGET ${CPPPS_LIB_TARGET})
    # message("Adding subdir: ${CPPPS_LIB_ABS_DIR}")
    add_subdirectory(${CPPPS_LIB_ABS_DIR} ${CPPPS_LIB_TARGET})
  endif()

  if (NOT TARGET ${CPPPS_LIB_ALIAS})
    # message("Adding alias ${CPPPS_LIB_ALIAS} of ${CPPPS_LIB_TARGET}")
    add_library(${CPPPS_LIB_ALIAS} ALIAS ${CPPPS_LIB_TARGET})
    set(CPPPS_LIBS "${CPPPS_LIBS} ${ALIAS}")
  endif()

endmacro()
