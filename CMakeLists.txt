
cmake_minimum_required (VERSION 2.8.11)

project(xWebProcessor)


message(STATUS "Module path: ${CMAKE_MODULE_PATH}")

# xsd dependencies
set ( XSD_INSTALL_INCLUDE_PATH CACHE STRING "xsd include files")
set ( XERCES_INSTALL_LIB_PATH CACHE STRING "xerces libraries files")


add_subdirectory(Src)
