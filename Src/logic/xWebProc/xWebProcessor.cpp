//
//  xWebProcessor.cpp
//  xWebProcessor
//
//  Created by Matthias Müller on 02.05.11.
//  Copyright 2011 Smartkinematics. All rights reserved.
//
#include <iostream>
#include <xsd/cxx/pre.hxx>
#include <boost/foreach.hpp>

#include "xWebMLProject.hxx"

#include "xWebProcessor.h"

#include "model/xWebProc/xWebProcessContext.h"
#include "logic/xWebProc/xWebFileItemProcessor.h"



xWebProcessor::xWebProcessor()
{

}

xWebProcessor::~xWebProcessor() {
}

void xWebProcessor::setProjectFilePath(std::string path) {
    _projectFilePath = path;
}

void xWebProcessor::setExecutableFilePath(std::string path)
{
    _executabelFilePath = path;
}

void xWebProcessor::addCLIStringParam(const std::string &key, const std::string &value)
{
    _cliStrings.insert(key, value);
}

bool xWebProcessor::run() {
    if ( boost::filesystem::exists(_projectFilePath) == false ) {
        std::cout << "Project file do not exists: " << _projectFilePath << std::endl;
        return false;
    }

    boost::filesystem::path projectfile = _projectFilePath;
    _baseFolder = boost::filesystem::canonical(projectfile.parent_path());

    std::cout << "Project root folder: " << _baseFolder << std::endl;

    boost::filesystem::path prevWorking = boost::filesystem::current_path();

    boost::filesystem::current_path(_baseFolder);

    try {
        _projectFile = xWebML::xWebProject(_projectFilePath);
    } catch (const xml_schema::exception& ex) {
        std::cout << "xml Parser error: ";
        std::cout << ex << std::endl;
        return false;
    }

    if ( false == deploySchemas() ) return false;

    if ( false == buildLanguageMap() ) return false;

    for ( int i=0 ; i<_languageMap.count(); i++)
    {
        std::string languageKey = _languageMap.languageKey(i);
        xWebStringList language = _languageMap.language(languageKey);

        xWebProcessContext context(*_projectFile, _baseFolder.string(), languageKey, language);
        context.getGlobalStrings()->override(_cliStrings);

        if ( prepareOutputFolder(context) == false ) {
            std::cout << "Prepare output folder faild: " << _projectFilePath<< std::endl;
            return false;
        }

        if ( processContent(context, _projectFile->Content()) == false ) {
            std::cout << "Process content faild: " << _projectFilePath << std::endl;
            return false;
        }
    }

    std::cout << "xWebProcessor succedded" << std::endl;

    boost::filesystem::current_path(prevWorking);

    return true;
}

bool xWebProcessor::deploySchemas()
{
    // deploy schemas
    boost::filesystem::path fileInfo(_projectFilePath);
    boost::filesystem::path schemaPath = fileInfo.parent_path();
    schemaPath /= "Schemas";
    if ( boost::filesystem::exists(schemaPath) == true ) {
        std::cout << "Empty schema folder: " << schemaPath << std::endl;
        boost::filesystem::remove_all(schemaPath);
    }


    boost::filesystem::create_directory( schemaPath);

    boost::filesystem::path sourceSchemaPath;
    std::list<boost::filesystem::path> searchPath;
    searchPath.push_back(boost::filesystem::path(_executabelFilePath).parent_path() / "../Resources/Schemas");// mac app bundle package
    searchPath.push_back(boost::filesystem::path(_executabelFilePath).parent_path() / "../../../../share/xWebProcessor/Schemas");// mac dev env

    std::list<boost::filesystem::path>::const_iterator it;
    bool found = false;
    for ( it = searchPath.begin(); it != searchPath.end(); it++ )
    {
        if ( boost::filesystem::exists(*it) == true ) {
            std::cout << "Schema source folder found: " << *it << std::endl;
            sourceSchemaPath = *it;
            found = true;
            break;
        }
    }

    if ( found == false )
    {
        std::cout << "ERROR: schema path not found" << std::endl;
        return false;
    }


    boost::filesystem::copy( sourceSchemaPath / "xWebMLStringList.xsd", schemaPath / "xWebMLStringList.xsd");
    boost::filesystem::copy( sourceSchemaPath / "xWebMLProject.xsd",    schemaPath / "xWebMLProject.xsd");
    boost::filesystem::copy( sourceSchemaPath / "xWebMLTemplate.xsd",   schemaPath / "xWebMLTemplate.xsd");
    boost::filesystem::copy( sourceSchemaPath / "xWebMLLinkList.xsd",   schemaPath / "xWebMLLinkList.xsd");

    return true;
}

bool xWebProcessor::buildLanguageMap()
{
    boost::filesystem::path baseFolderPath = _baseFolder;
    boost::filesystem::path contentFolderPath = baseFolderPath / _projectFile->Settings().ContentFolder().get();

    if ( _projectFile->Settings().ContentFolder().present() == true )
        _languageMap.buildLanguageMap( contentFolderPath.string() );
    else
        std::cout << "No content folder specified." << std::endl;

    return true;
}

bool xWebProcessor::prepareOutputFolder(xWebProcessContext& context) {
    std::cout << "Prepare outputfolder" << std::endl;

    context.initCurrentFolder();

    std::cout << "Outputfolder: " << context.currentFolder() << std::endl;

    std::string outputFolderPath = context.currentFolder();

    if ( boost::filesystem::exists(outputFolderPath) == false )
        boost::filesystem::create_directory(outputFolderPath);

    return true;
}

bool xWebProcessor::processContent(xWebProcessContext& context, xWebML::FolderType& root) {
    // process static folders
    {
        xWebML::Children::StaticFolder_iterator it = root.Children().StaticFolder().begin();

        while ( it != root.Children().StaticFolder().end() ) {
            xWebML::StaticFolderType xmlStaticFolder = *it;

            processStaticFolder(context, xmlStaticFolder);

            it++;
        }

    }

    // process normal folders
    {
        xWebML::Children::Folder_iterator it = root.Children().Folder().begin();

        while ( it != root.Children().Folder().end() ) {
            xWebML::FolderType xmlFolder = *it;

            processFolder(context, xmlFolder);

            it++;
        }
    }

    // process file items
    {
        xWebML::Children::FileItem_iterator it = root.Children().FileItem().begin();

        while ( it != root.Children().FileItem().end() ) {
            xWebML::FileItemType xmlFileItem = *it;

            processFileItem(context, xmlFileItem);

            it++;
        }
    }

    return true;
}

bool xWebProcessor::processFolder(xWebProcessContext& context, xWebML::FolderType& folder) {

    std::string folderName = context.expandTemplate(folder.Name() );

    context.enqueueFolder(folderName);

    processContent(context, folder);

    context.dequeueFolder();

    return true;
}

bool xWebProcessor::processStaticFolder(xWebProcessContext& context, xWebML::StaticFolderType& staticFolder) {

    std::string folderName = staticFolder.Name();
    boost::filesystem::path currentFolder;

    try
    {
        if ( folderName.compare(".") == 0 )
            currentFolder= boost::filesystem::path(context.currentFolder()) ;
        else
            currentFolder= boost::filesystem::path(context.currentFolder()) / folderName ;
    }
    catch (boost::filesystem::filesystem_error const &ex)
    {
        std::cout << "ERROR: " << ex.what() << std::endl;
        return false;
    }

    if ( boost::filesystem::exists(currentFolder.normalize() ) == true )
    {
        try
        {
            boost::filesystem::remove_all(currentFolder);
        }
        catch (boost::filesystem::filesystem_error const &ex)
        {
            std::cout << "ERROR: " << ex.what() << std::endl;
            return false;
        }
    }

    //boost::filesystem::create_directory(currentFolder);

    boost::filesystem::path sourceFolder = boost::filesystem::canonical(
                boost::filesystem::path(context.workingFolder()) / staticFolder.SourceFolder() );

    if ( !copyFolder(sourceFolder, currentFolder) )
        return false;

    return true;
}

bool xWebProcessor::processFileItem(xWebProcessContext& context, xWebML::FileItemType& fileItem) {

    xWebFileItemProcessor fileItemProcessor(fileItem);
    fileItemProcessor.run(context);

    return true;
}

bool xWebProcessor::copyFolder(boost::filesystem::path sourcefolder, boost::filesystem::path destFolder) {
    try
    {
        if ( !boost::filesystem::exists(sourcefolder) || !boost::filesystem::is_directory(sourcefolder)) {
            std::cout << "Source folder " << sourcefolder.string() << " do not exists or is not a directory." << std::endl;
            return false;
        }

        if ( boost::filesystem::exists(destFolder) ) {
            std::cout << "Destination folder " << destFolder.string() << " already exists." << std::endl;
            return false;
        }

        if ( !boost::filesystem::create_directory(destFolder)) {
            std::cout << "Unable to create destination directory " << destFolder.string() << std::endl;
            return false;
        }
    }
    catch (boost::filesystem::filesystem_error const &ex ) {
        std::cout << "ERROR: " << ex.what() << std::endl;
        return false;
    }

    // Iterate through the source directory
    for( boost::filesystem::directory_iterator file(sourcefolder); file != boost::filesystem::directory_iterator(); ++file ) {

        try {
            boost::filesystem::path current = file->path();

            if ( boost::filesystem::is_directory(current) == true ) {
                if ( !copyFolder(current, destFolder / current.filename()) )
                    return false;
            }
            else {
                boost::filesystem::copy(current, destFolder / current.filename());
            }
        }
        catch (boost::filesystem::filesystem_error const &ex ) {
            std::cout << "ERROR: " << ex.what() << std::endl;
            return false;
        }
    }

    return true;
}
