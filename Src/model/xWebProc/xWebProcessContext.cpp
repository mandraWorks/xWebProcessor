//
//  xWebProcessContext.cpp
//  xWebProcessor
//
//  Created by Matthias Müller on 02.05.11.
//  Copyright 2011 Smartkinematics. All rights reserved.
//

#include <xsd/cxx/pre.hxx>
#include <ctemplate/template.h>

#include "xWebMLProject.hxx"

#include <iostream>

#include "xWebProcessContext.h"
#include "model/xWebProc/xWebStringList.h"


xWebProcessContext::xWebProcessContext(xWebML::Settings& settings) :
  _localStrings(0), _activeMenuIDs(0)
{

  _outputFolder = QString(settings.OutputFolder().c_str());

  _globalStrings  = new xWebStringList(settings.StringList());
  _content        = 0;
}

xWebProcessContext::~xWebProcessContext() {

  delete _globalStrings;

  if ( _content != 0 )
    delete _content;
}

void xWebProcessContext::setCurrentContent(QString contentFile) {
  if ( _content == 0 )
    _content = new xWebStringList(contentFile);
  else
    _content->init(contentFile);
}

void xWebProcessContext::setCurrentContent(xWebML::StringListType& list) {
  if ( _content == 0 )
    _content = new xWebStringList(list);
  else
    _content->init(list);
}

void xWebProcessContext::setContentPrefix(QString str) {
  _contentPrefix = str;
}

void xWebProcessContext::setActiveMenuIDs(QStringList array) {
  _activeMenuIDs = array;
}

QString xWebProcessContext::getString(QString key) const {
  if ( _localStrings != 0 ) {
    if ( _localStrings->contains(key) == true )
      return _localStrings->stringForKey(key);
  }

  if ( _globalStrings->contains(key) == true )
    return _globalStrings->stringForKey(key);

  return QString();
}

QString xWebProcessContext::getContent(QString key) const {
  if ( _content != 0 ) {
    if ( _content->contains(key) == true ) {
      return _content->stringForKey(key);
    }
  }

  return QString();
}

QString xWebProcessContext::workingFolder() const {

  return QDir::currentPath();
}

void xWebProcessContext::initCurrentFolder() {
  QString currentFolder = QString("%1/%2").arg(workingFolder()).arg(_outputFolder);

  QDir dir;
  dir.mkpath(currentFolder);

  _currentFolder.append(currentFolder);
}

void xWebProcessContext::enqueueFolder(QString folder) {

  QString newFolderPath = QString("%1/%2").arg(currentFolder()).arg(folder);

  QDir dir;
  dir.mkpath(newFolderPath);

  std::cout << "New folder: " << newFolderPath.toLocal8Bit().constData() << std::endl;

  _currentFolder.append(newFolderPath);
}

void xWebProcessContext::dequeueFolder() {
  _currentFolder.removeLast();
}

QString xWebProcessContext::currentFolder() {    
  return _currentFolder.last();
}

QString xWebProcessContext::expandTemplate(QString templ) {
  ctemplate::TemplateDictionary dict("File");

  for ( int i=0; i<getGlobalStrings()->keys().count(); i++) {
      QString key = getGlobalStrings()->keys().at(i);
      QString value = getGlobalStrings()->stringForKey(key);
      dict.SetValue( key.toLocal8Bit().constData(), value.toLocal8Bit().constData());
  }

  if ( getLocalStrings()->contains("BaseName")==true )
      dict.SetValue("BaseName", getLocalStrings()->stringForKey("BaseName").toLocal8Bit().constData());
  if ( getLocalStrings()->contains("Language")==true )
      dict.SetValue("Language", getLocalStrings()->stringForKey("Language").toLocal8Bit().constData());

  time_t rawtime;
  struct tm * timeinfo;

  time( &rawtime);
  timeinfo = localtime(&rawtime);

  char buffer[80];
  strftime(buffer, 80, "%H:%M:%S %d.%m.%Y", timeinfo);

  dict.SetValue("CurrentTime", buffer);

  ctemplate::Template* tpl = ctemplate::Template::StringToTemplate(templ.toLocal8Bit().constData(), ctemplate::DO_NOT_STRIP);

  std::string output;
  tpl->Expand(&output, &dict);

  return QString::fromStdString(output);
}
