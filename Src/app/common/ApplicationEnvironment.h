#ifndef APPLICATIONENVIRONMENT_H
#define APPLICATIONENVIRONMENT_H

#include "mandraworks/core/system/Environment.h"


class ApplicationEnvironment : public mandraworks::core::system::Environment
{
public:
  ApplicationEnvironment();

  QString appName() const;
};

#endif // APPLICATIONENVIRONMENT_H
