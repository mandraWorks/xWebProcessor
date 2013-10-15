
set( xWebProcessor_SRCS
  ../Src_autogen/xWebMLLinkList.cxx
  ../Src_autogen/xWebMLProject.cxx
  ../Src_autogen/xWebMLStringList.cxx
  ../Src_autogen/xWebMLTEmplate.cxx
  main.cpp
)

set( xWebProcessor_HDRS
  ../Src_autogen/xWebMLLinkList.hxx
  ../Src_autogen/xWebMLProject.hxx
  ../Src_autogen/xWebMLStringList.hxx
  ../Src_autogen/xWebMLTEmplate.hxx
)

add_executable(xWebProcessor
  ${xWebProcessor_SRCS}
)
