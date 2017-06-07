CMAKE_MINIMUM_REQUIRED(VERSION 3.3)

include(${CMAKE_CURRENT_SOURCE_DIR}/config.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/conf.d/cmake/common.cmake)

# Bindings to compile
# --------------------
project_subdirs_add()

project_targets_populate()
project_package_build()

project_closing_msg()

