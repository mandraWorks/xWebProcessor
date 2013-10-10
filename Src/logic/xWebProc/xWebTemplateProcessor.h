//
//  xWebTemplateProcessor.h
//  xWebProcessor
//
//  Created by Matthias Müller on 06.05.11.
//  Copyright 2011 Smartkinematics. All rights reserved.
//

#include "xWebTemplateParser.h"
#include "xWebMLLinkList.hxx"

class xWebProcessContext;

class xWebTemplateProcessor : public xWebTemplateParserDelegate {
    
public:
    xWebTemplateProcessor(xWebProcessContext* context);
    ~xWebTemplateProcessor();
    
    void processString(std::string xmlData, std::ofstream& outStream);
    void processContentLink(std::string xmlData, std::ofstream& outStream);
    void processContent(std::string xmlData, std::ofstream& outStream);
    void processMenu(std::string xmlData, std::ofstream& outStream);
    void processIncludeFile(std::string xmlData, std::ofstream& outStream);

private:
    void processSubMenu(xWebML::LinkListType& xmlSubLinks, std::string templateFileName, std::string& output);

    void logException(const xml_schema::exception& ex);
    
private:
    xWebProcessContext* _context;
};
