!contains( MY_INCLUDE_PRIFILES, modelxWebProc) {

MY_INCLUDE_PRIFILES += modelxWebProc


HEADERS +=  $${SRCPATH}/model/xWebProc/xWebStringList.h \
			$${SRCPATH}/model/xWebProc/xWebProcessContext.h \
			$${SRCPATH}/model/xWebProc/xWebStringsParser.h \


SOURCES +=  $${SRCPATH}/model/xWebProc/xWebStringList.cpp \
			$${SRCPATH}/model/xWebProc/xWebProcessContext.cpp \
			$${SRCPATH}/model/xWebProc/xWebStringsParser.cpp \


message( "files: modelxWebProc")
}
