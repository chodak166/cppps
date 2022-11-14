include(${CMAKE_CURRENT_LIST_DIR}/cppps_module_helper.cmake)
CPPPS_ADD_MODULE("logging" cppps-logging-static cppps::logging)
CPPPS_ADD_MODULE("logging" cppps-logging-shared cppps::logging-shared)
