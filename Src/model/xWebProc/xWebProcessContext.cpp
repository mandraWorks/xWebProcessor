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


xWebProcessContext::xWebProcessContext(xWebML::ProjectType& project, std::string workingFolder, std::string language, const xWebStringList &languageStrings) :
    _workingFolder(workingFolder), _localStrings(0), _activeMenuIDs(0), _language(language)
{

    _outputFolder = project.Settings().OutputFolder();

    _globalStrings  = new xWebStringList(project.Settings().StringList());
    _languageStrings  = new xWebStringList(languageStrings);
    _content        = 0;

    _globalStrings->insert("Language", language);
}

xWebProcessContext::~xWebProcessContext() {

    delete _globalStrings;
    delete _languageStrings;

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

void xWebProcessContext::fillStringDict(ctemplate::TemplateDictionary *dict)
{
    for ( _globalStrings->init(); _globalStrings->more(); _globalStrings->next()) {
        std::string key = _globalStrings->key();
        std::string value = _globalStrings->value();
        dict->SetValue( key, value);
    }

    for ( _languageStrings->init(); _languageStrings->more(); _languageStrings->next()) {
        std::string key = _languageStrings->key();
        std::string value = _languageStrings->value();
        dict->SetValue( key, value);
    }

    if ( _localStrings->contains("BaseName")==true )
        dict->SetValue("BaseName", _localStrings->stringForKey("BaseName") );
    if ( _language.length() != 0 )
        dict->SetValue("Language", _language );
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

    for ( _globalStrings->init(); _globalStrings->more(); _globalStrings->next()) {
        std::string key = _globalStrings->key();
        std::string value = _globalStrings->value();
        dict.SetValue( key, value);
    }

    if ( _localStrings != 0 ) {
        if ( _localStrings->contains("BaseName")==true )
            dict.SetValue("BaseName", _localStrings->stringForKey("BaseName"));
        if ( _localStrings->contains("Language")==true )
            dict.SetValue("Language", _localStrings->stringForKey("Language"));
    }

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
