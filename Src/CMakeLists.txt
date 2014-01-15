set( SRCAUTOGEN_PATH ${xWebProcessor_BINARY_DIR}/Src_autogen )

set( schemas
  xWebMLLinkList
  xWebMLProject
  xWebMLStringList
  xWebMLTemplate
)

set( generated_cxx_targets )

foreach( s ${schemas} )
  add_custom_command( OUTPUT ${SRCAUTOGEN_PATH}/${s}.cxx
    COMMAND ${CMAKE_COMMAND} -E make_directory ${SRCAUTOGEN_PATH}
    COMMAND ${XSD_EXECUTABLE} cxx-tree --generate-serialization --output-dir ${SRCAUTOGEN_PATH} ${xWebProcessor_SOURCE_DIR}/Schemas/${s}.xsd
    DEPENDS ${xWebProcessor_SOURCE_DIR}/Schemas/${s}.xsd
    COMMENT "Generate cxx source for ${s}"
  )
  set( generated_cxx_targets ${generated_cxx_targets} generate_${s} )
endforeach()

set( xWebProcessor_SRCS
  ${SRCAUTOGEN_PATH}/xWebMLLinkList.cxx
  ${SRCAUTOGEN_PATH}/xWebMLProject.cxx
  ${SRCAUTOGEN_PATH}/xWebMLStringList.cxx
  ${SRCAUTOGEN_PATH}/xWebMLTemplate.cxx
  model/xWebProc/xWebProcessContext.cpp
  model/xWebProc/xWebStringList.cpp
  model/xWebProc/xWebStringsParser.cpp
  logic/xWebProc/xWebFileItemProcessor.cpp
  logic/xWebProc/xWebProcessor.cpp
  logic/xWebProc/xWebTemplateParser.cpp
  logic/xWebProc/xWebTemplateProcessor.cpp
  main.cpp
)

set( xWebProcessor_HDRS
  ${SRCAUTOGEN_PATH}/xWebMLLinkList.hxx
  ${SRCAUTOGEN_PATH}/xWebMLProject.hxx
  ${SRCAUTOGEN_PATH}/xWebMLStringList.hxx
  ${SRCAUTOGEN_PATH}/xWebMLTemplate.hxx
  model/xWebProc/xWebProcessContext.h
  model/xWebProc/xWebStringList.h
  model/xWebProc/xWebStringsParser.h
  logic/xWebProc/xWebFileItemProcessor.h
  logic/xWebProc/xWebProcessor.h
  logic/xWebProc/xWebTemplateParser.h
  logic/xWebProc/xWebTemplateProcessor.h
)

include_directories(
  ${SRCAUTOGEN_PATH}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set( _type )
if( APPLE )
  set( _type MACOSX_BUNDLE )
endif()

add_executable(xWebProcessor ${_type}
  ${xWebProcessor_SRCS}
)
target_link_libraries(xWebProcessor
  ${XERCESC_LIBRARY}
  ${Boost_LIBRARIES}
  ${CTEMPLATE_LIBRARIES}
)

add_dependencies( xWebProcessor ${generated_cxx_targets} )

install( TARGETS xWebProcessor
  RUNTIME DESTINATION ${INSTALL_BIN_DIR}
  BUNDLE  DESTINATION ${INSTALL_BIN_DIR}
)

set( CPACK_GENERATOR "TBZ2" )
set( CPACK_PACKAGE_NAME "xWebProcessor" )
set( CPACK_PACKAGE_VERSION_MAJOR "${xWebProcessor_MAJOR_VERSION}" )
set( CPACK_PACKAGE_VERSION_MINOR "${xWebProcessor_MINOR_VERSION}" )
set( CPACK_PACKAGE_VERSION_PATCH "${xWebProcessor_PATCH_VERSION}" )
set( CPACK_PACKAGE_VERSION "${WebProcessor_VERSION}" )
set( CPACK_PACKAGE_VENDOR "mandraWorks" )
set( CPACK_RESOURCE_FILE_README "${xWebProcessor_SOURCE_DIR}/ReadMe.md" )
set( CPACK_PACKAGE_FILE_NAME "xWebProcessor-${xWebProcessor_VERSION}")

if( APPLE )
  set( CPACK_GENERATOR "DragNDrop" )
  set( CPACK_DMG_FORMAT "UDBZ" )
  set( CPACK_DMG_VOLUME_NAME "xWebProcessor" )
  set( CPACK_SYSTEM_NAME "OSX" )
endif()

include( CPack )
