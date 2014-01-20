//
//  xWebProcessContext.cpp
//  xWebProcessor
//
//  Created by Matthias Müller on 02.05.11.
//  Copyright 2011 Smartkinematics. All rights reserved.
//

#include <xsd/cxx/pre.hxx>
#include <ctemplate/template.h>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string/predicate.hpp>

#include "xWebMLProject.hxx"

#include <iostream>

#include "xWebProcessContext.h"
#include "model/xWebProc/xWebStringList.h"


xWebProcessContext::xWebProcessContext(xWebML::ProjectType& project, std::string workingFolder) :
  _workingFolder(workingFolder), _localStrings(0), _activeMenuIDs(0)
{

  _outputFolder = project.Settings().OutputFolder();

  _globalStrings  = new xWebStringList(project.Settings().StringList());
  _content        = 0;

  if ( project.Settings().ContentFolder().present() == true )
      buildLanguageMap(project.Settings().ContentFolder().get());
  else
      std::cout << "No content folder specified." << std::endl;
}

xWebProcessContext::~xWebProcessContext() {

  delete _globalStrings;

  if ( _content != 0 )
    delete _content;
}

void xWebProcessContext::setCurrentContent(std::string contentFile) {
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

void xWebProcessContext::setContentPrefix(std::string str) {
  _contentPrefix = str;
}

void xWebProcessContext::setActiveMenuIDs(std::list<std::string> array) {
  _activeMenuIDs = array;
}

std::string xWebProcessContext::getString(std::string key) const {
  if ( _localStrings != 0 ) {
    if ( _localStrings->contains(key) == true )
      return _localStrings->stringForKey(key);
  }

  if ( _globalStrings->contains(key) == true )
    return _globalStrings->stringForKey(key);

  return "";
}

std::string xWebProcessContext::getContent(std::string key) const {
  if ( _content != 0 ) {
    if ( _content->contains(key) == true ) {
      return _content->stringForKey(key);
    }
  }

  return "";
}

std::string xWebProcessContext::resolveString(std::string value) const
{
    std::string prefix = "///string:";
    if(value.substr(0, prefix.size()) == prefix) {
        std::string key = value.substr(prefix.size());
        return _globalStrings->stringForKey(key);
    }
    else
        return value;
}

std::string xWebProcessContext::workingFolder() const {
    return _workingFolder;
}

void xWebProcessContext::initCurrentFolder() {
  boost::filesystem::path currentFolder = boost::filesystem::path(workingFolder()) / _outputFolder;

  boost::filesystem::create_directory(currentFolder);

  _currentFolder.push_back(currentFolder.string());
}

void xWebProcessContext::enqueueFolder(std::string folder) {

  boost::filesystem::path newFolderPath = boost::filesystem::path(currentFolder()) / folder;

  boost::filesystem::create_directory(newFolderPath);

  std::cout << "New folder: " << newFolderPath << std::endl;

  _currentFolder.push_back(newFolderPath.string());
}

void xWebProcessContext::dequeueFolder() {
  _currentFolder.pop_back();
}

std::string xWebProcessContext::currentFolder() {
  return _currentFolder.back();
}

std::string xWebProcessContext::expandTemplate(std::string templ) {
  ctemplate::TemplateDictionary dict("File");

  for ( getGlobalStrings()->init(); getGlobalStrings()->more(); getGlobalStrings()->next()) {
      std::string key = getGlobalStrings()->key();
      std::string value = getGlobalStrings()->value();
      dict.SetValue( key, value);
  }

  if ( getLocalStrings()->contains("BaseName")==true )
      dict.SetValue("BaseName", getLocalStrings()->stringForKey("BaseName"));
  if ( getLocalStrings()->contains("Language")==true )
      dict.SetValue("Language", getLocalStrings()->stringForKey("Language"));

  time_t rawtime;
  struct tm * timeinfo;

  time( &rawtime);
  timeinfo = localtime(&rawtime);

  char buffer[80];
  strftime(buffer, 80, "%H:%M:%S %d.%m.%Y", timeinfo);

  dict.SetValue("CurrentTime", buffer);

  ctemplate::Template* tpl = ctemplate::Template::StringToTemplate(templ, ctemplate::DO_NOT_STRIP);

  std::string output;
  tpl->Expand(&output, &dict);

  return output;
}

void xWebProcessContext::buildLanguageMap(std::string contentFolder)
{
    _languageKeys.clear();
    _languages.clear();

    boost::filesystem::path contentFolderPath = contentFolder;

    if ( boost::filesystem::exists(contentFolderPath) == false )
    {
        std::cout << "ERROR: Contentfolder do not exists: " << contentFolder << std::endl;
        return;
    }

    boost::filesystem::directory_iterator end_it;
    for ( boost::filesystem::directory_iterator it(contentFolderPath);
          it != end_it; it++)
    {
        if ( boost::filesystem::is_directory(it->status()) == true )
        {
            boost::filesystem::path languageFolder = *it;
            std::string languageKey = languageFolder.filename().string();
            std::cout << languageKey << std::endl;
        }
    }
}
