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
    xWebStringList();
    xWebStringList(std::string contentFile);
    xWebStringList(xWebML::StringListType& list);
    ~xWebStringList();
    
    void init(std::string contentFile);
    void init(xWebML::StringListType& list);

    void appendContentFile(std::string contentFile, bool prefix);

    void insert(const std::string& key, const std::string& value);
    void override( const xWebStringList& other );
    
    bool contains(std::string key) const;
    std::string stringForKey(std::string key) const;

    void init();
    bool more() const;
    void next();

    std::string key();
    std::string value();

    void dump();
    
private:
    Container _data;
    Container::iterator _iterator;
};
