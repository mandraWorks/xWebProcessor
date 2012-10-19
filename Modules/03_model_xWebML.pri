!contains( MY_INCLUDE_PRIFILES, modelxWebML) {

MY_INCLUDE_PRIFILES += modelxWebML


HEADERS +=  $${AUTOGEN_SRCPATH}/xWebMLStringList.hxx \
			$${AUTOGEN_SRCPATH}/xWebMLProject.hxx \
			$${AUTOGEN_SRCPATH}/xWebMLTemplate.hxx \
                        $${AUTOGEN_SRCPATH}/xWebMLLinkList.hxx \


SOURCES +=  $${AUTOGEN_SRCPATH}/xWebMLStringList.cxx \
			$${AUTOGEN_SRCPATH}/xWebMLProject.cxx \
			$${AUTOGEN_SRCPATH}/xWebMLTemplate.cxx \
			$${AUTOGEN_SRCPATH}/xWebMLLinkList.cxx \


OTHER_FILES += $${SCHEMAPATH}/xWebMLStringList.xsd \
			   $${SCHEMAPATH}/xWebMLProject.xsd \
			   $${SCHEMAPATH}/xWebMLTemplate.xsd \
			   $${SCHEMAPATH}/xWebMLLinkList.xsd \

RESOURCES += $${SCHEMAPATH}/Schemas.qrc


message( "files: modelxWebML")
}
