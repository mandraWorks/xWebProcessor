//
//  xWebTemplateProcessor.cpp
//  xWebProcessor
//
//  Created by Matthias Müller on 06.05.11.
//  Copyright 2011 Smartkinematics. All rights reserved.
//
#include <xsd/cxx/pre.hxx>
#include <ctemplate/template.h>

#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

#include "xWebMLTemplate.hxx"
#include "xWebMLLinkList.hxx"
#include "xWebMLStringList.hxx"

#include <sstream>
#include <string>

#include "xWebTemplateProcessor.h"


#include "model/xWebProc/xWebProcessContext.h"
#include "model/xWebProc/xWebStringList.h"


xWebTemplateProcessor::xWebTemplateProcessor(xWebProcessContext* context) :
    _context(context)
{

}

xWebTemplateProcessor::~xWebTemplateProcessor() {

}

void xWebTemplateProcessor::processString(std::string xmlData, std::ofstream& outStream) {
    std::string startTags = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
            "<xWeb:String xmlns:xWeb=\"http://www.pspsmartsoft.com/xWebML\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:schemaLocation=\"http://www.pspsmartsoft.com/xWebML Schemas/xWebMLTemplate.xsd\">";
    std::string xmlCompleteData = xmlData;

    boost::algorithm::replace_all( xmlCompleteData, "<xWeb:String>", startTags);

    std::stringstream stream(xmlCompleteData);

    try {
        std::auto_ptr<xWebML::StringType> xmlString = xWebML::String(stream);

        outStream << _context->getString(xmlString->c_str());

    } catch (const xml_schema::exception& ex) {
        logException(ex);
        return;
    }
}

void xWebTemplateProcessor::processContentLink(std::string xmlData, std::ofstream& outStream) {
    std::string startTags = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
            "<xWeb:ContentLink xmlns:xWeb=\"http://www.pspsmartsoft.com/xWebML\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:schemaLocation=\"http://www.pspsmartsoft.com/xWebML Schemas/xWebMLTemplate.xsd\">";
    std::string xmlCompleteData = xmlData;

    boost::algorithm::replace_all( xmlCompleteData, "<xWeb:ContentLink>", startTags);

    std::stringstream stream(xmlCompleteData);

    try {
        std::auto_ptr<xWebML::ContentLinkType> xmlContentLink = xWebML::ContentLink(stream);

        std::string folder = xmlContentLink->Folder().c_str();
        std::string fileName = xmlContentLink->FileName().c_str();
        std::string contentFileName = _context->workingFolder();

        contentFileName.append("/");
        contentFileName.append(folder);
        contentFileName.append(_context->contentPrefix());
        contentFileName.append(fileName);

        _context->setCurrentContent(contentFileName);

    } catch (const xml_schema::exception& ex) {
        logException(ex);
        return;
    }

}

void xWebTemplateProcessor::processContent(std::string xmlData, std::ofstream& outStream) {
    std::string startTags = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
            "<xWeb:Content xmlns:xWeb=\"http://www.pspsmartsoft.com/xWebML\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:schemaLocation=\"http://www.pspsmartsoft.com/xWebML Schemas/xWebMLTemplate.xsd\">";
    std::string xmlCompleteData = xmlData;

    boost::algorithm::replace_all(xmlCompleteData, "<xWeb:Content>", startTags);

    std::stringstream stream(xmlCompleteData);

    try {
        std::auto_ptr<xWebML::ContentType> xmlContent = xWebML::Content(stream);

        outStream << _context->getContent(xmlContent->c_str());

    } catch (const xml_schema::exception& ex) {
        logException(ex);
        return;
    }
}


void xWebTemplateProcessor::processMenu(std::string xmlData, std::ofstream& outStream) {
    std::string startTags = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
            "<xWeb:Menu xmlns:xWeb=\"http://www.pspsmartsoft.com/xWebML\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:schemaLocation=\"http://www.pspsmartsoft.com/xWebML Schemas/xWebMLTemplate.xsd\">";
    std::string xmlCompleteData = xmlData;

    boost::algorithm::replace_all(xmlCompleteData, "<xWeb:Menu>", startTags);

    std::stringstream stream(xmlCompleteData);

    try {
        std::auto_ptr<xWebML::MenuType> xmlMenu = xWebML::Menu(stream);

        std::string templateFileName = xmlMenu->TemplateFile().c_str();
        std::string linkListFileName = xmlMenu->LinkListFile().c_str();

        std::string subTemplateFileName;

        if ( xmlMenu->SubTemplateFile().present() == true ) {
            subTemplateFileName = xmlMenu->SubTemplateFile().get().c_str();
        }
        else {
            subTemplateFileName = templateFileName;
        }

        std::string linkListFile = _context->workingFolder();
        linkListFile.append("/");
        linkListFile.append(linkListFileName);

        std::string templateFile = _context->workingFolder();
        templateFile.append("/");
        templateFile.append(templateFileName);

        std::auto_ptr<xWebML::LinkListType> xmlLinkList = xWebML::LinkList(linkListFile);

        xWebML::LinkListType::LinkEntry_iterator it = xmlLinkList->LinkEntry().begin();

        ctemplate::TemplateDictionary dictLink("Link");
        if ( _context->getLocalStrings()->contains("BaseName")==true )
            dictLink.SetValue("BaseName", _context->getLocalStrings()->stringForKey("BaseName"));
        if ( _context->getLocalStrings()->contains("Language")==true )
            dictLink.SetValue("Language", _context->getLocalStrings()->stringForKey("Language"));

        while ( it != xmlLinkList->LinkEntry().end() ) {
            ctemplate::TemplateDictionary dict("LinkEntry");

            std::string link = it->Link().c_str();

            ctemplate::Template* linkTpl = ctemplate::Template::StringToTemplate(link.c_str(), link.length(), ctemplate::DO_NOT_STRIP);

            std::string linkOutput;
            linkTpl->Expand(&linkOutput, &dictLink);

            dict.SetValue("Link", linkOutput);
            dict.SetValue("Label", _context->getContent(it->StringKey().c_str()));

            if ( it->Image().present() == true ) {
                dict.SetValue("Image", it->Image().get().c_str());
                dict.ShowSection("Img_Sec");
            }

            if ( it->SubLinks().present() == true ) {
                xWebML::LinkListType xmlSubLinkList = it->SubLinks().get();
                std::string subLinks;
                processSubMenu(xmlSubLinkList, subTemplateFileName, subLinks);
                dict.SetValue("SubLinks", subLinks);
                dict.ShowSection("Sub_Sec");
            }

            std::string id = it->ID();

            std::list<std::string> activeMenuIDs = _context->activeMenuIDs();
            if ( (activeMenuIDs.size() != 0) && (std::find( activeMenuIDs.begin(), activeMenuIDs.end(), id) != activeMenuIDs.end() ) )
                dict.ShowSection("Act_Sec");

            it++;

            if ( it != xmlLinkList->LinkEntry().end() )
                dict.ShowSection("Sep_Sec");

            ctemplate::Template* tpl = ctemplate::Template::GetTemplate(templateFile, ctemplate::DO_NOT_STRIP);

            std::string output;
            tpl->Expand(&output, &dict);

            outStream << output.c_str() << "\n";
        }

    } catch (const xml_schema::exception& ex) {
        logException(ex);
        return;
    }
}

void xWebTemplateProcessor::processIncludeFile(std::string xmlData, std::ofstream& outStream) {
    std::string startTags = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
            "<xWeb:IncludeFile xmlns:xWeb=\"http://www.pspsmartsoft.com/xWebML\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:schemaLocation=\"http://www.pspsmartsoft.com/xWebML Schemas/xWebMLTemplate.xsd\">";
    std::string xmlCompleteData = xmlData;

    boost::algorithm::replace_all(xmlCompleteData, "<xWeb:IncludeFile>", startTags);

    std::stringstream stream(xmlCompleteData);

    try {
        std::auto_ptr<xWebML::IncludeFileType> xmlIncludeFile = xWebML::IncludeFile(stream);

        std::string includeFileName = xmlIncludeFile->File();
        includeFileName = _context->expandTemplate(includeFileName);

        std::string includeFilePath = _context->workingFolder();
        includeFilePath.append("/");
        includeFilePath.append(includeFileName);

        boost::filesystem::path includeFile = includeFilePath;

        if ( boost::filesystem::exists(includeFile) == false ) {
            std::cout << " Include file do not exists: " << includeFilePath << std::endl;
            return;
        }

        std::ifstream file;
        file.open (includeFile.string().c_str(), std::ifstream::in);

        if ( !file ) {
            std::cout << " Could not open include file: " << includeFilePath << std::endl;
            return;
        }

        std::string line;

        xWebTemplateParser parser;
        parser.setDelegate(this);
        parser.init();

        while (!file.eof()) {
            std::getline(file, line);
            parser.parseLine(line, outStream);
        };

        file.close();

    } catch (const xml_schema::exception& ex) {
        logException(ex);
        return;
    }
}

void xWebTemplateProcessor::processSubMenu(xWebML::LinkListType& xmlSubLinks, std::string templateFileName, std::string& output) {
    std::stringstream stream(output);
    try {

        std::string templateFile = _context->workingFolder();
        templateFile.append("/");
        templateFile.append(templateFileName);

        xWebML::LinkListType::LinkEntry_iterator it = xmlSubLinks.LinkEntry().begin();

        ctemplate::TemplateDictionary dictLink("Link");
        if ( _context->getLocalStrings()->contains("BaseName")==true )
            dictLink.SetValue("BaseName", _context->getLocalStrings()->stringForKey("BaseName"));
        if ( _context->getLocalStrings()->contains("Language")==true )
            dictLink.SetValue("Language", _context->getLocalStrings()->stringForKey("Language"));

        while ( it != xmlSubLinks.LinkEntry().end() ) {
            ctemplate::TemplateDictionary dict("LinkEntry");

            std::string link =it->Link().c_str();

            ctemplate::Template* linkTpl = ctemplate::Template::StringToTemplate(link.c_str(), link.length(), ctemplate::DO_NOT_STRIP);

            std::string linkOutput;
            linkTpl->Expand(&linkOutput, &dictLink);

            dict.SetValue("Link", linkOutput);
            dict.SetValue("Label", _context->getContent(it->StringKey()));

            if ( it->Image().present() == true ) {
                dict.SetValue("Image", it->Image().get().c_str());
                dict.ShowSection("Img_Sec");
            }

            if ( it->SubLinks().present() == true ) {
                xWebML::LinkListType xmlSubLinkList = it->SubLinks().get();
                std::string subLinks;
                processSubMenu(xmlSubLinkList, templateFileName, subLinks);
                dict.SetValue("SubLinks", subLinks);
                dict.ShowSection("Sub_Sec");
            }

            std::string id = it->ID();

            std::list<std::string> activeMenuIDs = _context->activeMenuIDs();
            if ( (activeMenuIDs.size() != 0) && (std::find( activeMenuIDs.begin(), activeMenuIDs.end(), id) != activeMenuIDs.end() ) )
                dict.ShowSection("Act_Sec");

            it++;

            if ( it != xmlSubLinks.LinkEntry().end() )
                dict.ShowSection("Sep_Sec");

            ctemplate::Template* tpl = ctemplate::Template::GetTemplate(templateFile, ctemplate::DO_NOT_STRIP);

            std::string temploutput;
            tpl->Expand(&temploutput, &dict);

            stream << temploutput.c_str() << "\n";
        }

    }
    catch (const xml_schema::exception& ex) {
        logException(ex);
        return;
    }

    output = stream.str();
}


void xWebTemplateProcessor::logException(const xml_schema::exception& ex) {
    std::stringstream stream;
    stream << ex;
    std::string str = stream.str();
    std::vector<std::string> lines;
    std::vector<std::string>::iterator line;
    boost::algorithm::split( lines, str, boost::algorithm::is_any_of("\n"), boost::algorithm::token_compress_off);

    for( line=lines.begin(); line!=lines.end(); line++ ) {
        std::cout << *line << std::endl;
    }
}
