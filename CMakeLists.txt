
cmake_minimum_required (VERSION 2.8.11)

project(xWebProcessor)

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

add_subdirectory(Src)
