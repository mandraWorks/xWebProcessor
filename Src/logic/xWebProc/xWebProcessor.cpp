//
//  xWebProcessor.cpp
//  xWebProcessor
//
//  Created by Matthias Müller on 02.05.11.
//  Copyright 2011 Smartkinematics. All rights reserved.
//
#include <stream.h>
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

bool xWebProcessor::run() {
    if ( boost::filesystem::exists(_projectFilePath) == false ) {
        std::cout << "Project file do not exists: " << _projectFilePath << std::endl;
        return false;
    }

    boost::filesystem::path projectfile = _projectFilePath;
    boost::filesystem::path basefolder = boost::filesystem::canonical(projectfile.parent_path());

    std::cout << "Project root folder: " << basefolder << std::endl;

    boost::filesystem::path prevWorking = boost::filesystem::current_path();

    boost::filesystem::current_path(basefolder);

    // deploy schemas
    boost::filesystem::path fileInfo(_projectFilePath);
    boost::filesystem::path schemapath = fileInfo.parent_path();
    schemapath /= "Schemas";
    if ( boost::filesystem::exists(schemapath) == true ) {
        std::cout << "Empty schema folder: " << schemapath << std::endl;
        boost::filesystem::remove_all(schemapath);
    }


    boost::filesystem::create_directory( schemapath);

    boost::filesystem::path sourceSchema = "/Volumes/untitled/WorkMandraworks/Development/xWebProcessor/Schemas";

    boost::filesystem::copy( sourceSchema / "xWebMLStringList.xsd", schemapath / "xWebMLStringList.xsd");
    boost::filesystem::copy( sourceSchema / "xWebMLProject.xsd",    schemapath / "xWebMLProject.xsd");
    boost::filesystem::copy( sourceSchema / "xWebMLTemplate.xsd",   schemapath / "xWebMLTemplate.xsd");
    boost::filesystem::copy( sourceSchema / "xWebMLLinkList.xsd",   schemapath / "xWebMLLinkList.xsd");

    try {
        _projectFile = xWebML::xWebProject(_projectFilePath);
    } catch (const xml_schema::exception& ex) {
        std::cout << "xml Parser error: ";
        std::cout << ex << std::endl;
        return false;
    }


    xWebProcessContext context(_projectFile->Settings(), basefolder.string());

    if ( prepareOutputFolder(context) == false ) {
        std::cout << "Prepare output folder faild: " << _projectFilePath<< std::endl;
        return false;
    }

    if ( processContent(context, _projectFile->Content()) == false ) {
        std::cout << "Process content faild: " << _projectFilePath << std::endl;
        return false;
    }

    std::cout << "xWebProcessor succedded" << std::endl;

    boost::filesystem::current_path(prevWorking);

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

    std::string folderName = folder.Name();

    context.enqueueFolder(folderName);

    xWebML::Children::Folder_iterator it = folder.Children().Folder().begin();

    while ( it != folder.Children().Folder().end() ) {
        xWebML::FolderType xmlFolder = *it;

        processFolder(context, xmlFolder);

        it++;
    }

    context.dequeueFolder();

    return true;
}

bool xWebProcessor::processStaticFolder(xWebProcessContext& context, xWebML::StaticFolderType& staticFolder) {

    std::string folderName = staticFolder.Name();
    boost::filesystem::path currentFolder;

    try
    {
        currentFolder= boost::filesystem::path(context.currentFolder()) / folderName ;
    }
    catch (boost::filesystem::filesystem_error const &ex)
    {
        std::cout << "ERROR: " << ex.what() << std::endl;
        return false;
    }

    if ( boost::filesystem::exists(currentFolder) == true)
        boost::filesystem::remove_all(currentFolder);

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
