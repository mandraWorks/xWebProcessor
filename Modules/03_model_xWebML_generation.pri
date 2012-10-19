!contains( MY_INCLUDE_PRIFILES, modelxWebMLGen) {

MY_INCLUDE_PRIFILES += modelxWebMLGen


mapStringList.target = xWebMLStringList
mapStringList.commands = $${COMMONROOT}/Libraries/bin/xsd cxx-tree \
								--generate-serialization \
								--output-dir $${AUTOGEN_SRCPATH} \
								$${SCHEMAPATH}/xWebMLStringList.xsd
mapStringList.depends = FORCE


mapProject.target = xWebMLProject
mapProject.commands = $${COMMONROOT}/Libraries/bin/xsd cxx-tree \
								--generate-serialization \
								--output-dir $${AUTOGEN_SRCPATH} \
								$${SCHEMAPATH}/xWebMLProject.xsd
mapProject.depends = FORCE


mapTemplate.target = xWebMLTemplate
mapTemplate.commands = $${COMMONROOT}/Libraries/bin/xsd cxx-tree \
								--generate-serialization \
								--output-dir $${AUTOGEN_SRCPATH} \
								$${SCHEMAPATH}/xWebMLTemplate.xsd
mapTemplate.depends = FORCE


mapLinkList.target = xWebMLLinkList
mapLinkList.commands = $${COMMONROOT}/Libraries/bin/xsd cxx-tree \
								--generate-serialization \
								--output-dir $${AUTOGEN_SRCPATH} \
								$${SCHEMAPATH}/xWebMLLinkList.xsd
mapLinkList.depends = FORCE


QMAKE_EXTRA_TARGETS += mapStringList \
					   mapProject \
					   mapTemplate \
					   mapLinkList

PRE_TARGETDEPS += xWebMLStringList \
				  xWebMLProject \
				  xWebMLTemplate \
				  xWebMLLinkList \



message( "files: modelxWebMLGen")
}
