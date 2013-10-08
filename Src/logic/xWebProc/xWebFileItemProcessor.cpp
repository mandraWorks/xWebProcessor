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
#include <boost/filesystem.hpp>

#include "xWebMLProject.hxx"

#include <fstream>


#include "xWebFileItemProcessor.h"

#include "model/xWebProc/xWebProcessContext.h"
#include "model/xWebProc/xWebStringList.h"
#include "logic/xWebProc/xWebTemplateParser.h"
#include "logic/xWebProc/xWebTemplateProcessor.h"


xWebFileItemProcessor::xWebFileItemProcessor(xWebML::FileItemType& fileItem) :
    _strings(0)
{
    _processingMethod   = fileItem.ProcessingMethod();
    _sourceFilePath     = fileItem.SourceFilePath();
    _targetFileName     = fileItem.TargetFileName();

    if ( fileItem.OutputFolder().present() == true )
      _outputFolder = fileItem.OutputFolder().get().c_str();
    
    _contentPrefix.clear();
    
    if ( fileItem.Configuration().present() == true ) {
        if ( fileItem.Configuration().get().StringList().present() == true ) {
            _strings = new xWebStringList(fileItem.Configuration().get().StringList().get());
        }
        
        if ( fileItem.Configuration().get().ContentPrefix().present() == true ) {
            _contentPrefix = fileItem.Configuration().get().ContentPrefix().get();
        }
        
        if ( fileItem.Configuration().get().ActiveMenuIDs().present() == true ) {
            xWebML::ActiveMenuIDs xmlActiveMenuIDs = fileItem.Configuration().get().ActiveMenuIDs().get();
            
            xWebML::ActiveMenuIDs::MenuID_iterator it = xmlActiveMenuIDs.MenuID().begin();
            
            while ( it != xmlActiveMenuIDs.MenuID().end()) {
                std::string menuID = *it;
                _activeMenuIDs.push_back(menuID);
                
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
    
    boost::filesystem::path currentFolder;
    if ( _outputFolder.length() == 0 )
      currentFolder = context.currentFolder();
    else
      currentFolder = boost::filesystem::path(boost::filesystem::current_path()) / _outputFolder;

    if ( boost::filesystem::exists(currentFolder) == false )
      boost::filesystem::create_directory(currentFolder);

    boost::filesystem::path targetFile = currentFolder / _targetFileName;
    
    std::cout << "Generate file: " << targetFile << std::endl;
    
    boost::filesystem::path absSourceFilePath = boost::filesystem::path(context.workingFolder()) / _sourceFilePath;
    
    xWebTemplateProcessor processor(&context);
    xWebTemplateParser parser(QString::fromStdString(absSourceFilePath.string()), QString::fromStdString(targetFile.string()));
    parser.setDelegate(&processor);
    
    if ( false == parser.run() ) {
        releaseRun(context);
        return false;
    }
    
    ctemplate::TemplateDictionary dict("File");

    for ( context.getGlobalStrings()->init(); context.getGlobalStrings()->more(); context.getGlobalStrings()->next()) {
        std::string key = context.getGlobalStrings()->key();
        std::string value = context.getGlobalStrings()->value();
        dict.SetValue( key, value);
    }

    if ( context.getLocalStrings()->contains("BaseName")==true )
        dict.SetValue("BaseName", context.getLocalStrings()->stringForKey("BaseName"));
    if ( context.getLocalStrings()->contains("Language")==true )
        dict.SetValue("Language", context.getLocalStrings()->stringForKey("Language"));
    
    time_t rawtime;
    struct tm * timeinfo;
    
    time( &rawtime);
    timeinfo = localtime(&rawtime);
    
    char buffer[80];
    strftime(buffer, 80, "%H:%M:%S %d.%m.%Y", timeinfo);
    
    dict.SetValue("CurrentTime", buffer);
    
    ctemplate::Template* tpl = ctemplate::Template::GetTemplate(targetFile.string(), ctemplate::DO_NOT_STRIP);
    
    std::string output;
    tpl->Expand(&output, &dict);
    
    std::ofstream  outFile(targetFile.string().c_str(), std::ios_base::out | std::ios_base::trunc);
    
    outFile << output;
    
    outFile.close();
    
    releaseRun(context);
        
    return true;
}

void xWebFileItemProcessor::initRun(xWebProcessContext& context) {
    context.setLocalStrings(_strings);
    if ( _contentPrefix.size() != 0 )
        context.setContentPrefix(_contentPrefix);
    
    context.setActiveMenuIDs(_activeMenuIDs);
}

void xWebFileItemProcessor::releaseRun(xWebProcessContext& context) {
    context.setLocalStrings(0);
    context.setContentPrefix("");
    context.setActiveMenuIDs(std::list<std::string>());
}
