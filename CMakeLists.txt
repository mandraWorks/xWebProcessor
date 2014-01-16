cmake_minimum_required (VERSION 2.8.11)

project(xWebProcessor)

# version
set( xWebProcessor_MAJOR_VERSION 0)
set( xWebProcessor_MINOR_VERSION 2)
set( xWebProcessor_PATCH_VERSION 0)
set( xWebProcessor_VERSION ${xWebProcessor_MAJOR_VERSION}.${xWebProcessor_MINOR_VERSION}.${xWebProcessor_PATCH_VERSION} )

add_definitions( -DMAJOR_VERSION=${xWebProcessor_MAJOR_VERSION})
add_definitions( -DMINOR_VERSION=${xWebProcessor_MINOR_VERSION})
add_definitions( -DPATCH_VERSION=${xWebProcessor_PATCH_VERSION})

set( CMAKE_MODULE_PATH
  ${CMAKE_MODULE_PATH}
  ${xWebProcessor_SOURCE_DIR}/CMake
)

find_package( XercesC )
include_directories( ${XERCESC_INCLUDE_DIR} )

# boost
# set(Boost_NO_SYSTEM_PATHS ON)
# set(BOOST_SEARCH_PATH CACHE PATH "Boost search path")
# set(BOOST_ROOT ${BOOST_SEARCH_PATH})
find_package(Boost 1.54 REQUIRED COMPONENTS system filesystem program_options)
include_directories( ${Boost_INCLUDE_DIRS})

# xsd dependencies
find_package( XSD )
include_directories( ${XSD_INCLUDE_DIR})

#xerces dependency

#ctemplate dependency
find_package( Ctemplate )
include_directories( ${CTEMPLATE_INCLUDE_DIR} )

# output directories in the binary tree
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${xWebProcessor_BINARY_DIR}/bin )

# install directory
set( INSTALL_BIN_DIR bin CACHE PATH "Installation directory for the executable" )
set( INSTALL_SHARE_DIR share/xWebProcessor CACHE PATH "Installation directory for resources" )
set( INSTALL_BUNDLE_DIR . CACHE PATH "Installation directory for bundle" )

add_subdirectory(Src)
