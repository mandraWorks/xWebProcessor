#include "CommandLineParams.h"


namespace MMSmodel = mandraworks::core::ident;

template<>
MMSmodel::EnumType<CommandLineParams>::instances_list MMSmodel::EnumType<CommandLineParams>::s_instances=MMSmodel::EnumType<CommandLineParams>::instances_list();


static int n=0;

const CommandLineParams CommandLineParams::None(n++,"None");
const CommandLineParams CommandLineParams::ProjectFile(n++,"ProjectFile");


CommandLineParams::CommandLineParams(int aValue,const QString& aText):
  MMSmodel::EnumType<CommandLineParams>(aValue,aText)
{
}
