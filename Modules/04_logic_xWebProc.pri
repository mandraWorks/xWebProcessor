!contains( MY_INCLUDE_PRIFILES, logicxWebProc) {

MY_INCLUDE_PRIFILES += logicxWebProc


HEADERS +=  $${SRCPATH}/logic/xWebProc/xWebTemplateParser.h \
			$${SRCPATH}/logic/xWebProc/xWebTemplateProcessor.h \
			$${SRCPATH}/logic/xWebProc/xWebFileItemProcessor.h \
			$${SRCPATH}/logic/xWebProc/xWebProcessor.h \


SOURCES +=  $${SRCPATH}/logic/xWebProc/xWebTemplateParser.cpp \
			$${SRCPATH}/logic/xWebProc/xWebTemplateProcessor.cpp \
			$${SRCPATH}/logic/xWebProc/xWebFileItemProcessor.cpp \
			$${SRCPATH}/logic/xWebProc/xWebProcessor.cpp \


message( "files: logicxWebProc")
}
