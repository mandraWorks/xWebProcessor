//
//  xWebProcessContext.h
//  xWebProcessor
//
//  Created by Matthias Müller on 02.05.11.
//  Copyright 2011 Smartkinematics. All rights reserved.
//

#include <string>
#include <list>
#include <deque>
#include <ctemplate/template.h>

namespace xWebML {
class Settings;
class ProjectType;
class StringListType;
}
class xWebStringList;

class xWebProcessContext {
public:
  xWebProcessContext(xWebML::ProjectType& project, std::string workingFolder, std::string language, const xWebStringList& languageStrings);
  virtual ~xWebProcessContext();

  void setLocalStrings(xWebStringList* list) { _localStrings = list; }
  void setCurrentContent(std::string contentFile);
  void setCurrentContent(xWebML::StringListType& list);
  void setContentPrefix(std::string str);
  void setActiveMenuIDs(std::list<std::string> array);

  std::string outputFolder() const { return _outputFolder; }

  void fillStringDict( ctemplate::TemplateDictionary* dict );

  xWebStringList* getGlobalStrings() const { return _globalStrings; }
  xWebStringList* getLocalStrings() const { return _localStrings; }
  xWebStringList* getLanguageStrings() const { return _languageStrings; }

  std::string getString(std::string key) const;
  std::string getContent(std::string key) const;

  std::string workingFolder() const;

  std::string contentPrefix() const { return _contentPrefix; }
  std::list<std::string>  activeMenuIDs() const { return _activeMenuIDs; }

  void initCurrentFolder();

  void enqueueFolder(std::string folder);
  void dequeueFolder();

  std::string currentFolder();

  std::string expandTemplate(std::string templ);

private:
  std::string _workingFolder;
  std::string _outputFolder;
  std::deque<std::string>  _currentFolder;

  xWebStringList* _globalStrings;
  xWebStringList* _localStrings;
  xWebStringList* _languageStrings;

  std::string     _contentPrefix;
  xWebStringList* _content;

  std::list<std::string>      _activeMenuIDs;

  std::string _language;
};
