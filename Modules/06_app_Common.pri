!contains( MY_INCLUDE_PRIFILES, appCommon) {

MY_INCLUDE_PRIFILES += appCommon


HEADERS +=  $${SRCPATH}/app/common/ApplicationEnvironment.h \
            $${SRCPATH}/app/common/CommandLineParams.h \


SOURCES +=  $${SRCPATH}/app/common/ApplicationEnvironment.cpp \
            $${SRCPATH}/app/common/CommandLineParams.cpp \


message( "files: appCommon")
}
