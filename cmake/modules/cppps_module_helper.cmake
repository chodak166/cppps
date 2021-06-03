macro(PB_ADD_MODULE PB_LIB_DIR PB_LIB_TARGET PB_LIB_ALIAS)

  if (NOT COMMON_DIR)
    get_filename_component(COMMON_DIR ${CMAKE_CURRENT_LIST_DIR}/../.. ABSOLUTE)
  endif()

  set(PB_LIB_ABS_DIR "${COMMON_DIR}/${PB_LIB_DIR}")

  if (NOT TARGET ${PB_LIB_TARGET})
    add_subdirectory(${PB_LIB_ABS_DIR} ${PB_LIB_TARGET})
    add_library(${PB_LIB_ALIAS} ALIAS ${PB_LIB_TARGET})
    set(PB_LIBS "${PB_LIBS} ${ALIAS}")
  endif()

endmacro()
