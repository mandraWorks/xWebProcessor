
isEmpty(PREFIX){
    PREFIX = $${COMMONROOT}/Libraries
}
message(Installation prefix: $${PREFIX})

OTHER_FILES +=  $${APPROOT}/Modules/07_foundation_install.list \
                $${APPROOT}/Modules/07_foundation_install_debug.list \
                $${APPROOT}/Deployment/info.plist

INSTALLDIR = $${PREFIX}/bin

#install bin
target.path = $${INSTALLDIR}
INSTALLS += target


#link libraries
CONFIG(debug, debug|release) {
	LISTFILE = 07_foundation_install_debug.list
} else {
	LISTFILE = 07_foundation_install.list
}

targetLinkLib.path = $${INSTALLDIR}/$${TARGET}.app
targetLinkLib.extra += $${PREFIX}/bin/macBundleLibs.app/Contents/MacOS/macBundleLibs --AppBundle=$${INSTALLDIR}/$${TARGET}.app --LibList=$${APPROOT}/Modules/$${LISTFILE}

INSTALLS += targetLinkLib


#Copy plist file
targetPList.path = $${INSTALLDIR}/$${TARGET}.app/Contents/
targetPList.files = $${APPROOT}/Deployment/info.plist

INSTALLS += targetPList


#Fillout plist file
targetPInfo.path = $${INSTALLDIR}/$${TARGET}.app/Contents
targetPInfo.extra = /usr/libexec/PlistBuddy -c \"Set :CFBundleShortVersionString $${VERSION}\" $${targetPInfo.path}/Info.plist;
targetPInfo.extra += /usr/libexec/PlistBuddy -c \"Set :CFBundleGetInfoString $${TARGET} $${VERSION}\" $${targetPInfo.path}/Info.plist;
targetPInfo.extra += /usr/libexec/PlistBuddy -c \"Set :CFBundleDisplayName $${TARGET} $${VERSION}\" $${targetPInfo.path}/Info.plist

INSTALLS += targetPInfo


#Copy resources
targetResources.path = $${INSTALLDIR}/$${TARGET}.app/Contents/Resources
targetResources.files = $${APPROOT}/Deployment/logo.icns

INSTALLS += targetResources
