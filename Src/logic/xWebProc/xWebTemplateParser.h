//
//  xWebTemplateParser.h
//  xWebProcessor
//
//  Created by Matthias Müller on 06.05.11.
//  Copyright 2011 Smartkinematics. All rights reserved.
//
#ifndef XWEBPROCESSOR_HEADER
#define XWEBPROCESSOR_HEADER

#include <QtCore>

class xWebTemplateParserDelegate {
public:
    virtual void processString(QString xmlData, QTextStream& outStream) = 0;
    virtual void processContentLink(QString xmlData, QTextStream& outStream) = 0;
    virtual void processContent(QString xmlData, QTextStream& outStream) = 0;
    virtual void processMenu(QString xmlData, QTextStream& outStream) = 0;
};


class xWebTemplateParser {
    
    enum Parser_State {
      Idle,
      InTag  
    };
    
public:
    xWebTemplateParser(QString inputFilePath, QString outputFilePath);
    ~xWebTemplateParser();
    
    void setDelegate(xWebTemplateParserDelegate* delegate) { _delegate = delegate; }

    bool run();
    
private:
    void parseLine(QString line, QTextStream& outStream);
    void processCurrentElement(QTextStream& outStream);
    
    QString getStartTag(QString tag);
    QString getEndTag(QString tag);
    
private:
    QString _inputFilePath;
    QString _outputFilePath;
    QStringList _xWebTags;
    
    int _parserState;
    int _currentTagIndex;
    
    QString _currentElement;
    
    xWebTemplateParserDelegate* _delegate;
};

#endif
