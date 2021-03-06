//
//  xWebStringList.cpp
//  xWebProcessor
//
//  Created by Matthias Müller on 06.05.11.
//  Copyright 2011 Smartkinematics. All rights reserved.
//
#include <xsd/cxx/pre.hxx>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

#include "xWebMLStringList.hxx"


#include "xWebStringList.h"
#include "xWebStringsParser.h"


xWebStringList::xWebStringList()
{

}

xWebStringList::xWebStringList(std::string contentFile) {
    init(contentFile);
}

xWebStringList::xWebStringList(xWebML::StringListType& list) {
    init(list);
}

xWebStringList::~xWebStringList() {
}

void xWebStringList::init(std::string contentFile) {
    _data.clear();

    boost::filesystem::path contentFilePath = contentFile;

    std::string ext1 = boost::algorithm::to_lower_copy( contentFilePath.extension().string());
    std::string ext2 = boost::algorithm::to_lower_copy( contentFilePath.stem().extension().string());

    if ( (ext1.compare(".xml") == 0) && (ext2.compare(".xwebstringlist") == 0)) {
        std::auto_ptr<xWebML::StringListType> content = xWebML::StringList(contentFile);

        init(*content);
    }
    else if ( ext1.compare(".strings") == 0 ) {
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
        std::string key = it->Key();
        std::string value = it->Value();
        
        _data.insert(std::pair<std::string, std::string>(key, value));
        
        it++;
    }
}

void xWebStringList::appendContentFile(std::string contentFile, bool prefix)
{
    boost::filesystem::path contentFilePath = contentFile;

    std::string ext1 = boost::algorithm::to_lower_copy( contentFilePath.extension().string());
    std::string ext2 = boost::algorithm::to_lower_copy( contentFilePath.stem().extension().string());

    if ( (ext1.compare(".xml") == 0) && (ext2.compare(".xwebstringlist") == 0)) {
        //std::auto_ptr<xWebML::StringListType> content = xWebML::StringList(contentFile);

        //init(*content);
    }
    else if ( ext1.compare(".strings") == 0 ) {
        std::string basefilename = contentFilePath.stem().string();
        xWebStringsParser parser(contentFile);
        if ( prefix == true )
            parser.setKeyPrefix(basefilename);

        if ( parser.parse() == true ) {
            xWebStringList tempList;
            tempList._data = *(parser.data());
            this->override(tempList);
        }
    }
}

void xWebStringList::override(const xWebStringList &other)
{
    for ( Container::const_iterator it = other._data.begin(); it != other._data.end(); it++ )
    {
        if ( _data.find(it->first) != _data.end() )
            _data[it->first] = it->second;
        else
            _data.insert(it,it);
    }
    _data.insert( other._data.begin(), other._data.end());
}

void xWebStringList::insert(const std::string &key, const std::string &value)
{
    _data.insert(std::pair<std::string, std::string>(key, value));
}

bool xWebStringList::contains(std::string key) const {
    return _data.find(key) == _data.end() ? false : true ;
}

std::string xWebStringList::stringForKey(std::string key) const {
    if ( contains(key) == false )
        return "";

    std::string value = _data.at(key);
    
    return value;
}

void xWebStringList::init() {
    _iterator = _data.begin();
}

bool xWebStringList::more() const {
    return  _iterator == _data.end() ? false : true ;
}

void xWebStringList::next() {
    ++_iterator;
}

std::string xWebStringList::key() {
    return _iterator->first;
}

std::string xWebStringList::value() {
    return _iterator->second;
}

void xWebStringList::dump()
{
    std::cout << "Dump stringlist:" << std::endl;
    for ( init(); more(); next())
    {
        std::cout << key() << ":" << value() << std::endl;
    }
}
