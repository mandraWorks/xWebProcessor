//
//  xWebProcessor.cpp
//  xWebProcessor
//
//  Created by Matthias Müller on 02.05.11.
//  Copyright 2011 Smartkinematics. All rights reserved.
//
#include <stream.h>
#include <xsd/cxx/pre.hxx>
#include <boost/filesystem.hpp>

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

    // deploy schemas
    boost::filesystem::path fileInfo(_projectFilePath);
    boost::filesystem::path schemapath = fileInfo.parent_path();
    schemapath /= "Schemas";
    if ( boost::filesystem::exists(schemapath) == true ) {
        std::cout << "Empty schema folder: " << schemapath << std::endl;
        boost::filesystem::remove_all(schemapath);
    }


    boost::filesystem::create_directory( schemapath);

    boost::filesystem::path sourceSchema = "/Users/mmueller/WorkMandraWorks/Development/xWebProcessor/Schemas";

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


    xWebProcessContext context(_projectFile->Settings());

    if ( prepareOutputFolder(context) == false ) {
        std::cout << "Prepare output folder faild: " << _projectFilePath<< std::endl;
        return false;
    }

    if ( processContent(context, _projectFile->Content()) == false ) {
        std::cout << "Process content faild: " << _projectFilePath << std::endl;
        return false;
    }

    std::cout << "xWebProcessor succedded" << std::endl;

    return true;
}

bool xWebProcessor::prepareOutputFolder(xWebProcessContext& context) {
    std::cout << "Prepare outputfolder" << std::endl;

    context.initCurrentFolder();

    std::cout << "Outputfolder: " << context.currentFolder().toLocal8Bit().constData() << std::endl;

    QString outputFolderPath = context.currentFolder();

    //mandraworks::core::system::SystemLib::rmFolder(outputFolderPath);

    QDir dir(outputFolderPath);

    if ( dir.exists() == false )
        dir.mkpath(outputFolderPath);

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

    context.enqueueFolder(QString::fromStdString(folderName));

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

    QString folderName = QString(staticFolder.Name().c_str());
    QString currentFolder = QString("%1/%2").arg(context.currentFolder()).arg(folderName);

    QDir dir(currentFolder);

    if ( dir.exists())
        boost::filesystem::remove_all(currentFolder.toLocal8Bit().constData());

    dir.mkpath(currentFolder);

    QString sourceFolder = QString(staticFolder.SourceFolder().c_str());

    QDir dir2(sourceFolder);
    QFileInfoList entries = dir2.entryInfoList(QDir::AllEntries|QDir::NoDotAndDotDot);

    QString absSourceFolder = dir2.absolutePath();

    for ( int i=0; i<entries.count(); i++) {
        QFileInfo fileInfo = entries.at(i);

        QString itemPath = fileInfo.absoluteFilePath();
        QString relItemPath = itemPath.replace(absSourceFolder, "");

        QString newItemPath = QString("%1/%2").arg(currentFolder).arg(relItemPath);

        if ( fileInfo.isDir() == true ) {
            dir.mkpath(newItemPath);
            processStaticFolder(context, fileInfo.absoluteFilePath().toLocal8Bit().constData(), newItemPath.toLocal8Bit().constData());
        }
        else if ( fileInfo.isFile() == true ) {
            QFile::copy(fileInfo.absoluteFilePath(), newItemPath);
        }
    }

    return true;
}

bool xWebProcessor::processStaticFolder(xWebProcessContext& context, std::string sourceFolder, std::string outputFolder) {
    QString currentFolder = QString::fromStdString(sourceFolder);

    QDir dir;

    QDir dir2(QString::fromStdString(sourceFolder));
    QFileInfoList entries = dir2.entryInfoList(QDir::AllEntries|QDir::NoDotAndDotDot);

    QString absSourceFolder = dir2.absolutePath();

    for ( int i=0; i<entries.count(); i++) {
        QFileInfo fileInfo = entries.at(i);

        QString itemPath = fileInfo.absoluteFilePath();
        QString relItemPath = itemPath.replace(absSourceFolder, "");

        QString newItemPath = QString("%1/%2").arg(QString::fromStdString(outputFolder)).arg(relItemPath);

        if ( fileInfo.isDir() == true ) {
            dir.mkpath(newItemPath);
            processStaticFolder(context, fileInfo.absoluteFilePath().toLocal8Bit().constData(), newItemPath.toLocal8Bit().constData());
        }
        else if ( fileInfo.isFile() == true ) {
            QFile::copy(fileInfo.absoluteFilePath(), newItemPath);
        }
    }

    return true;
}

bool xWebProcessor::processFileItem(xWebProcessContext& context, xWebML::FileItemType& fileItem) {

    xWebFileItemProcessor fileItemProcessor(fileItem);
    fileItemProcessor.run(context);

    return true;
}
