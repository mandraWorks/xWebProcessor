
isEmpty(PREFIX){
    PREFIX = $${COMMONROOT}/Libraries
}
message(Installation prefix: $${PREFIX})


#install bin
target.path = $${PREFIX}/bin
INSTALLS += target
