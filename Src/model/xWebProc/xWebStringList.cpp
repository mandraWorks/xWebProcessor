//
//  xWebStringList.cpp
//  xWebProcessor
//
//  Created by Matthias Müller on 06.05.11.
//  Copyright 2011 Smartkinematics. All rights reserved.
//
#include <xsd/cxx/pre.hxx>

#include "xWebMLStringList.hxx"


#include "xWebStringList.h"
#include "xWebStringsParser.h"


xWebStringList::xWebStringList(QString contentFile) {
  init(contentFile);
}

xWebStringList::xWebStringList(xWebML::StringListType& list) {
  init(list);
}

xWebStringList::~xWebStringList() {
}

void xWebStringList::init(QString contentFile) {
    _data.clear();

    QFileInfo contentFileInfo(contentFile);

    if ( contentFileInfo.completeSuffix().compare("xwebstringlist.xml", Qt::CaseInsensitive) == 0 ) {
      std::auto_ptr<xWebML::StringListType> content = xWebML::StringList(contentFile.toLocal8Bit().constData());

      init(*content);
    }
    else if ( contentFileInfo.completeSuffix().compare("strings", Qt::CaseInsensitive) == 0 ) {
      xWebStringsParser parser(contentFile);

      _data.clear();
      if ( parser.parse() == true ) {
        _data = *(parser.data());
      }
    }
}

void xWebStringList::init(xWebML::StringListType& list) {
    _data.clear();
    xWebML::StringListType::Entry_iterator it = list.Entry().begin();
    
    while ( it != list.Entry().end() ) {
        QString key(it->Key().c_str());
        QString value = QString::fromUtf8(it->Value().c_str());
        
        _data.insert(key, value);
        
        it++;
    }
}

bool xWebStringList::contains(QString key) const {
  return _data.contains(key);
}

QString xWebStringList::stringForKey(QString key) const {
    if ( contains(key) == false )
      return QString();
    
    return _data[key];
}

QList<QString> xWebStringList::keys() const {
    return _data.keys();
}
