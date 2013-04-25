//
//  xWebProcessContext.h
//  xWebProcessor
//
//  Created by Matthias Müller on 02.05.11.
//  Copyright 2011 Smartkinematics. All rights reserved.
//

#include <QtCore>

namespace xWebML {
class Settings;
class StringListType;
}
class xWebStringList;

class xWebProcessContext {
public:
  xWebProcessContext(xWebML::Settings& settings);
  ~xWebProcessContext();

  void setLocalStrings(xWebStringList* list) { _localStrings = list; }
  void setCurrentContent(QString contentFile);
  void setCurrentContent(xWebML::StringListType& list);
  void setContentPrefix(QString str);
  void setActiveMenuIDs(QStringList array);

  QString outputFolder() const { return _outputFolder; }
  xWebStringList* getGlobalStrings() const { return _globalStrings; }
  xWebStringList* getLocalStrings() const { return _localStrings; }

  QString getString(QString key) const;
  QString getContent(QString key) const;

  QString workingFolder() const;

  QString contentPrefix() const { return _contentPrefix; }
  QStringList  activeMenuIDs() const { return _activeMenuIDs; }

  void initCurrentFolder();

  void enqueueFolder(QString folder);
  void dequeueFolder();

  QString currentFolder();

private:
  QString _outputFolder;
  QStringList  _currentFolder;

  xWebStringList* _globalStrings;
  xWebStringList* _localStrings;

  QString     _contentPrefix;
  xWebStringList* _content;

  QStringList      _activeMenuIDs;
};
