#include "ApplicationEnvironment.h"
#include "xWebProcessorDefs.h"


ApplicationEnvironment::ApplicationEnvironment()
{
}

QString ApplicationEnvironment::appName() const {
  return QString(APPLICATION_NAME);
}
