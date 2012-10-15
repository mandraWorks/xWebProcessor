#ifndef COMMANDLINEPARAMS_H
#define COMMANDLINEPARAMS_H

#include <QtCore>
#include "mandraworks/core/ident/EnumType.h"

class CommandLineParams : public mandraworks::core::ident::EnumType<CommandLineParams>
{
private:
    CommandLineParams(int aValue,const QString& aText);

public:
    static const CommandLineParams None;
    static const CommandLineParams ProjectFile;
};


#endif // COMMANDLINEPARAMS_H
