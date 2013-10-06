//
//  xWebTemplateParser.cpp
//  xWebProcessor
//
//  Created by Matthias Müller on 06.05.11.
//  Copyright 2011 Smartkinematics. All rights reserved.
//


#include <iostream>
#include <fstream>


#include "xWebTemplateParser.h"

#define STRING          "xWeb:String"
#define CONTENTLINK     "xWeb:ContentLink"
#define MENU            "xWeb:Menu"
#define CONTENT         "xWeb:Content"
#define INCLUDEFILE     "xWeb:IncludeFile"



xWebTemplateParser::xWebTemplateParser() :
    _delegate(0)
{

    _xWebTags.append(STRING);
    _xWebTags.append(CONTENTLINK);
    _xWebTags.append(CONTENT);
    _xWebTags.append(MENU);
    _xWebTags.append(INCLUDEFILE);
}


xWebTemplateParser::xWebTemplateParser(QString inputFilePath, QString outputFilePath) :
    _delegate(0), _inputFilePath(inputFilePath), _outputFilePath(outputFilePath)
{

    _xWebTags.append(STRING);
    _xWebTags.append(CONTENTLINK);
    _xWebTags.append(CONTENT);
    _xWebTags.append(MENU);
    _xWebTags.append(INCLUDEFILE);
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

    QFile sourceFile(_inputFilePath);
    QFile targetFile(_outputFilePath);

    if ( sourceFile.exists() == false ) {
        std::cout << " Source file do not exists: " << _inputFilePath.toLocal8Bit().constData() << std::endl;
        return false;
    }

    if ( sourceFile.open(QIODevice::ReadOnly|QIODevice::Text) == false ) {
        std::cout << " Could not open source file: " << _inputFilePath.toLocal8Bit().constData() << std::endl;
        return false;
    }

    if ( targetFile.open(QIODevice::WriteOnly|QIODevice::Truncate|QIODevice::Text) == false ) {
        std::cout << " Could not open target file: " << _outputFilePath.toLocal8Bit().constData() << std::endl;
        return false;
    }

    QString line;
    QTextStream sourceStream(&sourceFile);
    QTextStream targetStream(&targetFile);

    do {
        line = sourceStream.readLine();
        parseLine(line, targetStream);
    } while (!line.isNull());

    targetFile.close();
    sourceFile.close();

    return true;
}

void xWebTemplateParser::parseLine(QString line, QTextStream& outStream) {
    if ( _parserState == Idle ) {

        for ( int i=0; i<_xWebTags.count(); i++ ) {
            QString startTag = getStartTag(_xWebTags.at(i));
            if ( line.contains(startTag) == true ) {
                _currentElement.clear();

                _parserState = InTag;
                _currentTagIndex = i;

                int index = line.indexOf(startTag);

                outStream << line.mid(0, index);

                line = line.mid(index);

                // reneter state machine
                parseLine(line, outStream);
                return;
            }
        }

        outStream << line << "\n";;
    }
    else if ( _parserState == InTag ) {
        QString endTag = getEndTag(_xWebTags.at(_currentTagIndex));
        if ( line.contains(endTag) == true ) {

            int index = line.indexOf(endTag);

            _currentElement.append(line.mid(0, index + endTag.length()));

            processCurrentElement(outStream);

            outStream << line.mid(index + endTag.length());

            _parserState = Idle;
            _currentTagIndex = -1;
        }
        else {
            _currentElement.append(line);
        }
    }
}

void xWebTemplateParser::processCurrentElement(QTextStream& outStream) {
    QString currentTag = _xWebTags.at(_currentTagIndex);

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

QString xWebTemplateParser::getStartTag(QString tag) {
    QString str = QString("<%1>").arg(tag);
    return str;
}

QString xWebTemplateParser::getEndTag(QString tag) {
    QString str = QString("</%1>").arg(tag);
    return str;
}
