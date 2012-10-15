
TEMPLATE = app
LANGUAGE = C++

QT       += core xml

CONFIG +=console

CONFIG   -= app_bundle

TMPDIR = $${PWD}/tmp

DESTDIR = $${TMPDIR}
UI_DIR = $${TMPDIR}/Objs/UI
MOC_DIR = $${TMPDIR}/Objs/Objs
OBJECTS_DIR = $${TMPDIR}/Objs

TARGET = xWebProcessor

APPROOT = $${PWD}

VERSION_MAJOR=0
VERSION_MINOR=1
VERSION_PATCH=0

DEFINES += APP_VERSION_MAJOR=$${VERSION_MAJOR}
DEFINES += APP_VERSION_MINOR=$${VERSION_MINOR}
DEFINES += APP_VERSION_PATCH=$${VERSION_PATCH}
DEFINES += APPLICATION_NAME=\\\"$${TARGET}\\\"


VERSION = $${VERSION_MAJOR}.$${VERSION_MINOR}.$${VERSION_PATCH}

include($${APPROOT}/Modules/00_configEnv.pri)
include($${FRAMEWORKROOT}/Modules/01_foundation_MwCore.pri)
include($${FRAMEWORKROOT}/Modules/01_foundation_MwCmd.pri)

include($${PWD}/Modules/06_app_Common.pri)



HEADERS += $${SRCPATH}/xWebProcessorDefs.h

SOURCES += $${SRCPATH}/main.cpp
