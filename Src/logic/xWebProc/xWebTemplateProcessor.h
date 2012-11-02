//
//  xWebTemplateProcessor.h
//  xWebProcessor
//
//  Created by Matthias Müller on 06.05.11.
//  Copyright 2011 Smartkinematics. All rights reserved.
//

#include <QtCore>

#include "xWebTemplateParser.h"
#include "xWebMLLinkList.hxx"

class xWebProcessContext;

class xWebTemplateProcessor : public xWebTemplateParserDelegate {
    
public:
    xWebTemplateProcessor(xWebProcessContext* context);
    ~xWebTemplateProcessor();
    
    void processString(QString xmlData, QTextStream& outStream);
    void processContentLink(QString xmlData, QTextStream& outStream);
    void processContent(QString xmlData, QTextStream& outStream);
    void processMenu(QString xmlData, QTextStream& outStream);
    void processIncludeFile(QString xmlData, QTextStream& outStream);

private:
    void processSubMenu(xWebML::LinkListType& xmlSubLinks, QString templateFileName, QString& output);
    
private:
    xWebProcessContext* _context;
};
