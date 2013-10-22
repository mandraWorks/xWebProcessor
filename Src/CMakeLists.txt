
set( SRCAUTOGEN_PATH ../Src_autogen)

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

set( IncludeDirs
  ${SRCAUTOGEN_PATH}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

include_directories(${IncludeDirs})

add_executable(xWebProcessor
  ${xWebProcessor_SRCS}
)

target_link_libraries(xWebProcessor ${XERCERS_LIB} ${Boost_LIBRARIES} ${CTEMPLATE_LIB})
