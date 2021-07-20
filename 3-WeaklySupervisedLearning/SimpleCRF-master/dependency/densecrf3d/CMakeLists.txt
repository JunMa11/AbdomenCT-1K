cmake_minimum_required (VERSION 2.8)
project(denseCrf3d)

FIND_PACKAGE ( ITK REQUIRED)
INCLUDE( ${ITK_USE_FILE} )

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)

include_directories( include )

add_subdirectory (src)
add_subdirectory (applicationAndExamples)
add_subdirectory (external)
