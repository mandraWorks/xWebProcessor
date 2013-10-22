//
//  xWebStringList.h
//  xWebProcessor
//
//  Created by Matthias Müller on 06.05.11.
//  Copyright 2011 Smartkinematics. All rights reserved.
//
#include <iostream>
#include <string>
#include <map>

namespace xWebML {
    class StringListType;
}


class xWebStringList {
    typedef std::map<std::string,std::string> Container;
  
public:
  xWebStringList(std::string contentFile);
    xWebStringList(xWebML::StringListType& list);
    ~xWebStringList();
    
    void init(std::string contentFile);
    void init(xWebML::StringListType& list);
    
    bool contains(std::string key) const;
    std::string stringForKey(std::string key) const;

    void init();
    bool more() const;
    void next();

    std::string key();
    std::string value();
    
private:
    Container _data;
    Container::iterator _iterator;
};
