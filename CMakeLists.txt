
cmake_minimum_required (VERSION 2.8.11)

project(xWebProcessor)

# version
set ( xWebProcessor_MAJOR_VERSION 0)
set ( xWebProcessor_MINOR_VERSION 1)
set ( xWebProcessor_PATCH_VERSION 0)

add_definitions( -DMAJOR_VERSION=${xWebProcessor_MAJOR_VERSION})
add_definitions( -DMINOR_VERSION=${xWebProcessor_MINOR_VERSION})
add_definitions( -DPATCH_VERSION=${xWebProcessor_PATCH_VERSION})

# boost
set(Boost_NO_SYSTEM_PATHS ON)
set(BOOST_SEARCH_PATH CACHE PATH "Boost search path")
set(BOOST_ROOT ${BOOST_SEARCH_PATH})
find_package(Boost 1.54 REQUIRED system filesystem program_options)
include_directories( ${Boost_INCLUDE_DIRS})

# xsd dependencies
find_path( XSD_INC xercers XERCES_INSTALL_INCLUDE_PATH)
include_directories( ${XSD_INC})

#xerces dependency
find_path( XERCERS_INC xercers XERCES_INSTALL_INCLUDE_PATH)
find_library( XERCERS_LIB xercers XERCES_INSTALL_LIB_PATH)
include_directories( ${XERCERS_INC})

#ctemplate dependency
find_path( CTEMPLATE_INC ctemplate CTEMPLATE_INSTALL_INCLUDE_PATH)
find_library( CTEMPLATE_LIB xercers CTEMPLATE_INSTALL_LIB_PATH)
include_directories( ${CTEMPLATE_INC})

# output directories in the binary tree
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${xWebProcessor_BINARY_DIR}/bin)

# install directory
set (INSTALL_BIN_DIR bin CACHE PATH "INstallation directory for the executable")

add_subdirectory(Src)
