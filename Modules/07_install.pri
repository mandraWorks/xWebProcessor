
isEmpty(PREFIX){
    PREFIX = $${COMMONROOT}/Libraries
}
message(Installation prefix: $${PREFIX})

OTHER_FILES += $${APPROOT}/Modules/07_foundation_install.list \
			   $${APPROOT}/Modules/07_foundation_install_debug.list


#install bin
target.path = $${PREFIX}/bin
INSTALLS += target


#link libraries
CONFIG(debug, debug|release) {
	LISTFILE = 07_foundation_install_debug.list
} else {
	LISTFILE = 07_foundation_install.list
}

targetLinkLib.path = $${PREFIX}/bin/$${TARGET}.app
targetLinkLib.extra += $${PREFIX}/bin/macBundleLibs.app/Contents/MacOS/macBundleLibs --AppBundle=$${PREFIX}/bin/$${TARGET}.app --LibList=$${APPROOT}/Modules/$${LISTFILE}

INSTALLS += targetLinkLib
