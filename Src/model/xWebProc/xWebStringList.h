//
//  xWebStringList.h
//  xWebProcessor
//
//  Created by Matthias Müller on 06.05.11.
//  Copyright 2011 Smartkinematics. All rights reserved.
//
#include <iostream>
#include <QtCore>

namespace xWebML {
    class StringListType;
}


class xWebStringList {
  
public:
    xWebStringList(xWebML::StringListType& list);
    ~xWebStringList();
    
    void init(xWebML::StringListType& list);
    
    bool contains(QString key) const;
    QString stringForKey(QString key) const;
    
private:
    QMap<QString,QString> _data;
};