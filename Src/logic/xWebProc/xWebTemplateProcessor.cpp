//
//  xWebTemplateProcessor.cpp
//  xWebProcessor
//
//  Created by Matthias Müller on 06.05.11.
//  Copyright 2011 Smartkinematics. All rights reserved.
//
#include <xsd/cxx/pre.hxx>
#include <ctemplate/template.h>

#include "xWebMLTemplate.hxx"
#include "xWebMLLinkList.hxx"
#include "xWebMLStringList.hxx"

#include <sstream>
#include <string>

#include "xWebTemplateProcessor.h"

#include "mandraworks/core/log/Log.h"

#include "model/xWebProc/xWebProcessContext.h"
#include "model/xWebProc/xWebStringList.h"


xWebTemplateProcessor::xWebTemplateProcessor(xWebProcessContext* context) :
  _context(context)
{

}

xWebTemplateProcessor::~xWebTemplateProcessor() {

}

void xWebTemplateProcessor::processString(QString xmlData, QTextStream& outStream) {
  QString startTags = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
      "<xWeb:String xmlns:xWeb=\"http://www.pspsmartsoft.com/xWebML\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:schemaLocation=\"http://www.pspsmartsoft.com/xWebML Schemas/xWebMLTemplate.xsd\">";
  QString xmlCompleteData = xmlData;

  xmlCompleteData = xmlCompleteData.replace("<xWeb:String>", startTags);

  std::stringstream stream(xmlCompleteData.toLocal8Bit().constData());

  try {
    std::auto_ptr<xWebML::StringType> xmlString = xWebML::String(stream);

    QString key = QString(xmlString->c_str());

    outStream << _context->getString(key);

  } catch (const xml_schema::exception& ex) {
    std::cout << ex << std::endl;
    return;
  }
}

void xWebTemplateProcessor::processContentLink(QString xmlData, QTextStream& outStream) {
  QString startTags = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
      "<xWeb:ContentLink xmlns:xWeb=\"http://www.pspsmartsoft.com/xWebML\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:schemaLocation=\"http://www.pspsmartsoft.com/xWebML Schemas/xWebMLTemplate.xsd\">";
  QString xmlCompleteData = xmlData;

  xmlCompleteData = xmlCompleteData.replace("<xWeb:ContentLink>", startTags);

  std::stringstream stream(xmlCompleteData.toLocal8Bit().constData());

  try {
    std::auto_ptr<xWebML::ContentLinkType> xmlContentLink = xWebML::ContentLink(stream);

    QString folder = QString(xmlContentLink->Folder().c_str());
    QString fileName = QString(xmlContentLink->FileName().c_str());
    QString contentFileName = QString(_context->workingFolder());

    contentFileName.append("/");
    contentFileName.append(folder);
    contentFileName.append(_context->contentPrefix());
    contentFileName.append(fileName);

    std::auto_ptr<xWebML::StringListType> content = xWebML::StringList(contentFileName.toLocal8Bit().constData());

    _context->setCurrentContent(*content);

  } catch (const xml_schema::exception& ex) {
    std::cout << ex << std::endl;
    return;
  }

}

void xWebTemplateProcessor::processContent(QString xmlData, QTextStream& outStream) {
  QString startTags = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
      "<xWeb:Content xmlns:xWeb=\"http://www.pspsmartsoft.com/xWebML\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:schemaLocation=\"http://www.pspsmartsoft.com/xWebML Schemas/xWebMLTemplate.xsd\">";
  QString xmlCompleteData = xmlData;

  xmlCompleteData = xmlCompleteData.replace("<xWeb:Content>", startTags);

  std::stringstream stream(xmlCompleteData.toLocal8Bit().constData());

  try {
    std::auto_ptr<xWebML::ContentType> xmlContent = xWebML::Content(stream);

    QString key = QString(xmlContent->c_str());
    QString content = _context->getContent(key);

    outStream << content;

  } catch (const xml_schema::exception& ex) {
    std::cout << ex << std::endl;
    return;
  }
}


void xWebTemplateProcessor::processMenu(QString xmlData, QTextStream& outStream) {
  QString startTags = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
      "<xWeb:Menu xmlns:xWeb=\"http://www.pspsmartsoft.com/xWebML\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:schemaLocation=\"http://www.pspsmartsoft.com/xWebML Schemas/xWebMLTemplate.xsd\">";
  QString xmlCompleteData = xmlData;

  xmlCompleteData = xmlCompleteData.replace("<xWeb:Menu>", startTags);

  std::stringstream stream(xmlCompleteData.toLocal8Bit().constData());

  try {
    std::auto_ptr<xWebML::MenuType> xmlMenu = xWebML::Menu(stream);

    QString templateFileName = QString(xmlMenu->TemplateFile().c_str());
    QString linkListFileName = QString(xmlMenu->LinkListFile().c_str());

    QString subTemplateFileName;

    if ( xmlMenu->SubTemplateFile().present() == true ) {
      subTemplateFileName = QString(xmlMenu->SubTemplateFile().get().c_str());
    }
    else {
      subTemplateFileName = templateFileName;
    }

    QString linkListFile = _context->workingFolder();
    linkListFile.append("/");
    linkListFile.append(linkListFileName);

    QString templateFile = _context->workingFolder();
    templateFile.append("/");
    templateFile.append(templateFileName);

    std::auto_ptr<xWebML::LinkListType> xmlLinkList = xWebML::LinkList(linkListFile.toLocal8Bit().constData());

    xWebML::LinkListType::LinkEntry_iterator it = xmlLinkList->LinkEntry().begin();

    ctemplate::TemplateDictionary dictLink("Link");
    if ( _context->getLocalStrings()->contains("BaseName")==true )
      dictLink.SetValue("BaseName", _context->getLocalStrings()->stringForKey("BaseName").toLocal8Bit().constData());
    if ( _context->getLocalStrings()->contains("Language")==true )
      dictLink.SetValue("Language", _context->getLocalStrings()->stringForKey("Language").toLocal8Bit().constData());

    while ( it != xmlLinkList->LinkEntry().end() ) {
      ctemplate::TemplateDictionary dict("LinkEntry");

      QString stringKey = QString(it->StringKey().c_str());
      QString link = QString(it->Link().c_str());

      ctemplate::Template* linkTpl = ctemplate::Template::StringToTemplate(link.toLocal8Bit().constData(), link.length(), ctemplate::DO_NOT_STRIP);

      std::string linkOutput;
      linkTpl->Expand(&linkOutput, &dictLink);

      dict.SetValue("Link", linkOutput);
      dict.SetValue("Label", _context->getContent(stringKey).toLocal8Bit().constData());

      if ( it->Image().present() == true ) {
        dict.SetValue("Image", it->Image().get().c_str());
        dict.ShowSection("Img_Sec");
      }

      if ( it->SubLinks().present() == true ) {
        xWebML::LinkListType xmlSubLinkList = it->SubLinks().get();
        QString subLinks;
        processSubMenu(xmlSubLinkList, subTemplateFileName, subLinks);
        dict.SetValue("SubLinks", subLinks.toLocal8Bit().constData());
        dict.ShowSection("Sub_Sec");
      }

      QString id = QString(it->ID().c_str());

      if ( (_context->activeMenuIDs().length() != 0) && (_context->activeMenuIDs().contains(id) == true ) )
        dict.ShowSection("Act_Sec");

      it++;

      if ( it != xmlLinkList->LinkEntry().end() )
        dict.ShowSection("Sep_Sec");

      ctemplate::Template* tpl = ctemplate::Template::GetTemplate(templateFile.toLocal8Bit().constData(), ctemplate::DO_NOT_STRIP);

      std::string output;
      tpl->Expand(&output, &dict);

      outStream << QString::fromUtf8(output.c_str()) << "\n";
    }

  } catch (const xml_schema::exception& ex) {
    std::cout << ex << std::endl;
    return;
  }
}

void xWebTemplateProcessor::processSubMenu(xWebML::LinkListType& xmlSubLinks, QString templateFileName, QString& output) {
  QTextStream stream(&output);
  try {

    QString templateFile = _context->workingFolder();
    templateFile.append("/");
    templateFile.append(templateFileName);

    xWebML::LinkListType::LinkEntry_iterator it = xmlSubLinks.LinkEntry().begin();

    ctemplate::TemplateDictionary dictLink("Link");
    if ( _context->getLocalStrings()->contains("BaseName")==true )
      dictLink.SetValue("BaseName", _context->getLocalStrings()->stringForKey("BaseName").toLocal8Bit().constData());
    if ( _context->getLocalStrings()->contains("Language")==true )
      dictLink.SetValue("Language", _context->getLocalStrings()->stringForKey("Language").toLocal8Bit().constData());

    while ( it != xmlSubLinks.LinkEntry().end() ) {
      ctemplate::TemplateDictionary dict("LinkEntry");

      QString stringKey = QString(it->StringKey().c_str());
      QString link = QString(it->Link().c_str());

      ctemplate::Template* linkTpl = ctemplate::Template::StringToTemplate(link.toLocal8Bit().constData(), link.length(), ctemplate::DO_NOT_STRIP);

      std::string linkOutput;
      linkTpl->Expand(&linkOutput, &dictLink);

      dict.SetValue("Link", linkOutput);
      dict.SetValue("Label", _context->getContent(stringKey).toLocal8Bit().constData());

      if ( it->Image().present() == true ) {
        dict.SetValue("Image", it->Image().get().c_str());
        dict.ShowSection("Img_Sec");
      }

      if ( it->SubLinks().present() == true ) {
        xWebML::LinkListType xmlSubLinkList = it->SubLinks().get();
        QString subLinks;
        processSubMenu(xmlSubLinkList, templateFileName, subLinks);
        dict.SetValue("SubLinks", subLinks.toLocal8Bit().constData());
        dict.ShowSection("Sub_Sec");
      }

      QString id = QString(it->ID().c_str());

      if ( (_context->activeMenuIDs().length() != 0) && (_context->activeMenuIDs().contains(id) == true ) )
        dict.ShowSection("Act_Sec");

      it++;

      if ( it != xmlSubLinks.LinkEntry().end() )
        dict.ShowSection("Sep_Sec");

      ctemplate::Template* tpl = ctemplate::Template::GetTemplate(templateFile.toLocal8Bit().constData(), ctemplate::DO_NOT_STRIP);

      std::string output;
      tpl->Expand(&output, &dict);

      stream << QString::fromUtf8(output.c_str()) << "\n";
    }

  }
  catch (const xml_schema::exception& ex) {
    std::cout << ex << std::endl;
    return;
  }
}
