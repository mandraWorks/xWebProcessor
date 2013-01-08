//
//  xWebFileItemProcessor.cpp
//  xWebProcessor
//
//  Created by Matthias Müller on 05.05.11.
//  Copyright 2011 Smartkinematics. All rights reserved.
//
#include <xsd/cxx/pre.hxx>
#include <ctemplate/template.h>
#include <time.h>
#include <locale.h>

#include "xWebMLProject.hxx"

#include <fstream>


#include "xWebFileItemProcessor.h"

#include "mandraworks/core/log/Log.h"

#include "model/xWebProc/xWebProcessContext.h"
#include "model/xWebProc/xWebStringList.h"
#include "logic/xWebProc/xWebTemplateParser.h"
#include "logic/xWebProc/xWebTemplateProcessor.h"


xWebFileItemProcessor::xWebFileItemProcessor(xWebML::FileItemType& fileItem) :
    _strings(0)
{
    _processingMethod   = QString(fileItem.ProcessingMethod().c_str());
    _sourceFilePath     = QString(fileItem.SourceFilePath().c_str());
    _targetFileName     = QString(fileItem.TargetFileName().c_str());

    if ( fileItem.OutputFolder().present() == true )
      _outputFolder = fileItem.OutputFolder().get().c_str();
    
    _contentPrefix.clear();
    
    if ( fileItem.Configuration().present() == true ) {
        if ( fileItem.Configuration().get().StringList().present() == true ) {
            _strings = new xWebStringList(fileItem.Configuration().get().StringList().get());
        }
        
        if ( fileItem.Configuration().get().ContentPrefix().present() == true ) {
            _contentPrefix = QString(fileItem.Configuration().get().ContentPrefix().get().c_str());
        }
        
        if ( fileItem.Configuration().get().ActiveMenuIDs().present() == true ) {
            xWebML::ActiveMenuIDs xmlActiveMenuIDs = fileItem.Configuration().get().ActiveMenuIDs().get();
            
            xWebML::ActiveMenuIDs::MenuID_iterator it = xmlActiveMenuIDs.MenuID().begin();
            
            while ( it != xmlActiveMenuIDs.MenuID().end()) {
                QString menuID = QString(it->c_str());
                _activeMenuIDs.append(menuID);
                
                it++;
            }
        }
    }
}

xWebFileItemProcessor::~xWebFileItemProcessor() {
    
    if ( _strings != 0 )
        delete _strings;
}

bool xWebFileItemProcessor::run(xWebProcessContext& context) {
    if ( _processingMethod.compare("Transform") == 0 )
        return runTransform(context);
    
    return false;
}

bool xWebFileItemProcessor::runTransform(xWebProcessContext& context) {
    initRun(context);
    
    QString currentFolder;
    if ( _outputFolder.length() == 0 )
      currentFolder = context.currentFolder();
    else
      currentFolder = QString("%1/%2").arg(QDir::currentPath()).arg(_outputFolder);

    QDir dir(currentFolder);
    if ( dir.exists() == false )
      dir.mkpath(currentFolder);

    QString targetFile = QString("%1/%2").arg(currentFolder).arg(_targetFileName);
    
    mandraworks::core::log::Log::info(QString("Generate file: %1").arg(targetFile));
    
    QString absSourceFilePath = QString("%1/%2").arg(context.workingFolder()).arg(_sourceFilePath);
    
    xWebTemplateProcessor processor(&context);
    xWebTemplateParser parser(absSourceFilePath, targetFile);
    parser.setDelegate(&processor);
    
    if ( false == parser.run() ) {
        releaseRun(context);
        return false;
    }
    
    ctemplate::TemplateDictionary dict("File");

    for ( int i=0; i<context.getGlobalStrings()->keys().count(); i++) {
        QString key = context.getGlobalStrings()->keys().at(i);
        QString value = context.getGlobalStrings()->stringForKey(key);
        dict.SetValue( key.toLocal8Bit().constData(), value.toLocal8Bit().constData());
    }

    if ( context.getLocalStrings()->contains("BaseName")==true )
        dict.SetValue("BaseName", context.getLocalStrings()->stringForKey("BaseName").toLocal8Bit().constData());
    if ( context.getLocalStrings()->contains("Language")==true )
        dict.SetValue("Language", context.getLocalStrings()->stringForKey("Language").toLocal8Bit().constData());
    
    time_t rawtime;
    struct tm * timeinfo;
    
    time( &rawtime);
    timeinfo = localtime(&rawtime);
    
    char buffer[80];
    strftime(buffer, 80, "%H:%M:%S %d.%m.%Y", timeinfo);
    
    dict.SetValue("CurrentTime", buffer);
    
    ctemplate::Template* tpl = ctemplate::Template::GetTemplate(targetFile.toLocal8Bit().constData(), ctemplate::DO_NOT_STRIP);
    
    std::string output;
    tpl->Expand(&output, &dict);
    
    std::ofstream  outFile(targetFile.toLocal8Bit().constData(), std::ios_base::out | std::ios_base::trunc);
    
    outFile << output;
    
    outFile.close();
    
    releaseRun(context);
        
    return true;
}

void xWebFileItemProcessor::initRun(xWebProcessContext& context) {
    context.setLocalStrings(_strings);
    if ( _contentPrefix != 0 )
        context.setContentPrefix(_contentPrefix);
    
    context.setActiveMenuIDs(_activeMenuIDs);
}

void xWebFileItemProcessor::releaseRun(xWebProcessContext& context) {
    context.setLocalStrings(0);
    context.setContentPrefix("");
    context.setActiveMenuIDs(QStringList());
}
