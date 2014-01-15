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

namespace xWebML {
class Settings;
class StringListType;
}
class xWebStringList;

class xWebProcessContext {
public:
  xWebProcessContext(xWebML::Settings& settings, std::string workingFolder);
  ~xWebProcessContext();

  void setLocalStrings(xWebStringList* list) { _localStrings = list; }
  void setCurrentContent(std::string contentFile);
  void setCurrentContent(xWebML::StringListType& list);
  void setContentPrefix(std::string str);
  void setActiveMenuIDs(std::list<std::string> array);

  std::string outputFolder() const { return _outputFolder; }
  xWebStringList* getGlobalStrings() const { return _globalStrings; }
  xWebStringList* getLocalStrings() const { return _localStrings; }

  std::string getString(std::string key) const;
  std::string getContent(std::string key) const;
  std::string resolveString(std::string value) const;

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

  std::string     _contentPrefix;
  xWebStringList* _content;

  std::list<std::string>      _activeMenuIDs;
};
