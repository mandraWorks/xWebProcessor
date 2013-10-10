//
//  xWebTemplateParser.cpp
//  xWebProcessor
//
//  Created by Matthias Müller on 06.05.11.
//  Copyright 2011 Smartkinematics. All rights reserved.
//


#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>


#include "xWebTemplateParser.h"

#define STRING          "xWeb:String"
#define CONTENTLINK     "xWeb:ContentLink"
#define MENU            "xWeb:Menu"
#define CONTENT         "xWeb:Content"
#define INCLUDEFILE     "xWeb:IncludeFile"



xWebTemplateParser::xWebTemplateParser() :
    _delegate(0)
{

    _xWebTags.push_back(STRING);
    _xWebTags.push_back(CONTENTLINK);
    _xWebTags.push_back(CONTENT);
    _xWebTags.push_back(MENU);
    _xWebTags.push_back(INCLUDEFILE);
}


xWebTemplateParser::xWebTemplateParser(std::string inputFilePath, std::string outputFilePath) :
    _inputFilePath(inputFilePath), _outputFilePath(outputFilePath), _delegate(0)
{

    _xWebTags.push_back(STRING);
    _xWebTags.push_back(CONTENTLINK);
    _xWebTags.push_back(CONTENT);
    _xWebTags.push_back(MENU);
    _xWebTags.push_back(INCLUDEFILE);
}

xWebTemplateParser::~xWebTemplateParser() {

}

void xWebTemplateParser::init() {
    _parserState = Idle;
    _currentElement.clear();
}

bool xWebTemplateParser::run() {
    _parserState = Idle;
    _currentElement.clear();

    std::ifstream sourceFile;
    std::ofstream targetFile;

    if ( boost::filesystem::exists(_inputFilePath) == false ) {
        std::cout << " Source file do not exists: " << _inputFilePath << std::endl;
        return false;
    }

    sourceFile.open(_inputFilePath.c_str(), std::ios::in);
    if ( sourceFile.is_open() == false ) {
        std::cout << " Could not open source file: " << _inputFilePath << std::endl;
        return false;
    }

    targetFile.open(_outputFilePath.c_str(), std::ios::out | std::ios::trunc);
    if ( targetFile.is_open() == false ) {
        std::cout << " Could not open target file: " << _outputFilePath << std::endl;
        return false;
    }

    std::string line;

    while ( !sourceFile.eof() ) {
        std::getline(sourceFile, line);
        parseLine(line, targetFile);
    }

    targetFile.close();
    sourceFile.close();

    return true;
}

void xWebTemplateParser::parseLine(std::string line, std::ofstream& outStream) {
    if ( _parserState == Idle ) {

        std::list<std::string>::iterator it;
        for ( it = _xWebTags.begin(); it != _xWebTags.end(); ++it ) {
            std::string startTag = getStartTag(*it);
            if ( line.find(startTag) != std::string::npos ) {
                _currentElement.clear();

                _parserState = InTag;
                _currentTagIndex = it;

                std::string::size_type index = line.find(startTag);

                outStream << line.substr(0, index);

                line = line.substr(index);

                // reneter state machine
                parseLine(line, outStream);
                return;
            }
        }

        outStream << line << "\n";;
    }
    else if ( _parserState == InTag ) {
        std::string endTag = getEndTag(*_currentTagIndex);
        if ( line.find(endTag) != std::string::npos ) {

            std::string::size_type index = line.find(endTag);

            _currentElement.append(line.substr(0, index + endTag.length()));

            processCurrentElement(outStream);

            outStream << line.substr(index + endTag.length());

            _parserState = Idle;
            _currentTagIndex = _xWebTags.end();
        }
        else {
            _currentElement.append(line);
        }
    }
}

void xWebTemplateParser::processCurrentElement(std::ofstream& outStream) {
    std::string currentTag = *_currentTagIndex;

    if ( currentTag.compare(STRING) == 0 ) {
        if ( _delegate != 0 )
            _delegate->processString(_currentElement, outStream);
    }
    else if ( currentTag.compare(CONTENTLINK) == 0 ) {
        if ( _delegate != 0 )
            _delegate->processContentLink(_currentElement, outStream);
    }
    else if ( currentTag.compare(CONTENT) == 0 ) {
        if ( _delegate != 0 )
            _delegate->processContent(_currentElement, outStream);
    }
    else if ( currentTag.compare(MENU) == 0 ) {
        if ( _delegate != 0 )
            _delegate->processMenu(_currentElement, outStream);
    }
    else if ( currentTag.compare(INCLUDEFILE) == 0 ) {
        if ( _delegate != 0 )
            _delegate->processIncludeFile(_currentElement, outStream);
    }
}

std::string xWebTemplateParser::getStartTag(std::string tag) {
    std::string str = boost::str( boost::format("<%1%>") % tag);
    return str;
}

std::string xWebTemplateParser::getEndTag(std::string tag) {
    std::string str = boost::str( boost::format("</%1%>") % tag);
    return str;
}
