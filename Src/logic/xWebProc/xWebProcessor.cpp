//
//  xWebProcessor.cpp
//  xWebProcessor
//
//  Created by Matthias Müller on 02.05.11.
//  Copyright 2011 Smartkinematics. All rights reserved.
//
#include <stream.h>
#include <xsd/cxx/pre.hxx>
#include "xWebMLProject.hxx"

#include "xWebProcessor.h"

#include "mandraworks/core/log/Log.h"
#include "mandraworks/core/system/SystemLib.h"

#include "model/xWebProc/xWebProcessContext.h"
#include "logic/xWebProc/xWebFileItemProcessor.h"



xWebProcessor::xWebProcessor()
{

}

xWebProcessor::~xWebProcessor() {
}

void xWebProcessor::setProjectFilePath(QString path) {
    _projectFilePath = path;
}

bool xWebProcessor::run() {
    if ( QFile::exists(_projectFilePath) == false ) {
        mandraworks::core::log::Log::error(QString("Project file do not exists: %1").arg(_projectFilePath));
        return false;
    }

    // deploy schemas
    QFileInfo fileInfo(_projectFilePath);
    QDir schemaDir(fileInfo.absolutePath());
    if ( schemaDir.exists("Schemas") == true ) {
        mandraworks::core::system::SystemLib::rmFolder(QString("%1/Schemas").arg(fileInfo.absolutePath()));
    }

    schemaDir.mkdir("Schemas");
    schemaDir.cd("Schemas");

    QFile::copy(":/xWebMLStringList.xsd", QString("%1/xWebMLStringList.xsd").arg(schemaDir.absolutePath()));
    QFile::copy(":/xWebMLProject.xsd", QString("%1/xWebMLProject.xsd").arg(schemaDir.absolutePath()));
    QFile::copy(":/xWebMLTemplate.xsd", QString("%1/xWebMLTemplate.xsd").arg(schemaDir.absolutePath()));
    QFile::copy(":/xWebMLLinkList.xsd", QString("%1/xWebMLLinkList.xsd").arg(schemaDir.absolutePath()));

    try {
        _projectFile = xWebML::xWebProject(_projectFilePath.toLocal8Bit().constData());
    } catch (const xml_schema::exception& ex) {
      std::stringstream stream;
      stream << ex;
      QString str = QString::fromStdString(stream.str());
      mandraworks::core::log::Log::error(QString("xml Parser error: %1").arg( str.replace("\n","")));
        return false;
    }


    xWebProcessContext context(_projectFile->Settings());

    if ( prepareOutputFolder(context) == false ) {
        mandraworks::core::log::Log::error(QString("Prepare output folder faild: %1").arg(_projectFilePath));
        return false;
    }

    if ( processContent(context, _projectFile->Content()) == false ) {
        mandraworks::core::log::Log::error(QString("Process content faild: %1").arg(_projectFilePath));
        return false;
    }

    mandraworks::core::log::Log::info(QString("xWebProcessor succedded"));

    return true;
}

bool xWebProcessor::prepareOutputFolder(xWebProcessContext& context) {
    mandraworks::core::log::Log::info(QString("Prepare outputfolder"));

    context.initCurrentFolder();

    mandraworks::core::log::Log::info(QString("Outputfolder: %1").arg(context.currentFolder()));
    mandraworks::core::log::Log::info(QString("Clean outputfolder..."));

    QString outputFolderPath = context.currentFolder();

    mandraworks::core::system::SystemLib::rmFolder(outputFolderPath);

    QDir dir;

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

    QString folderName = QString(folder.Name().c_str());

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

    QString folderName = QString(staticFolder.Name().c_str());
    QString currentFolder = QString("%1/%2").arg(context.currentFolder()).arg(folderName);

    QDir dir;
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
            processStaticFolder(context, fileInfo.absoluteFilePath(), newItemPath);
        }
        else if ( fileInfo.isFile() == true ) {
            QFile::copy(fileInfo.absoluteFilePath(), newItemPath);
        }
    }

    return true;
}

bool xWebProcessor::processStaticFolder(xWebProcessContext& context, QString sourceFolder, QString outputFolder) {
    QString currentFolder = sourceFolder;

    QDir dir;

    QDir dir2(sourceFolder);
    QFileInfoList entries = dir2.entryInfoList(QDir::AllEntries|QDir::NoDotAndDotDot);

    QString absSourceFolder = dir2.absolutePath();

    for ( int i=0; i<entries.count(); i++) {
        QFileInfo fileInfo = entries.at(i);

        QString itemPath = fileInfo.absoluteFilePath();
        QString relItemPath = itemPath.replace(absSourceFolder, "");

        QString newItemPath = QString("%1/%2").arg(outputFolder).arg(relItemPath);

        if ( fileInfo.isDir() == true ) {
            dir.mkpath(newItemPath);
            processStaticFolder(context, fileInfo.absoluteFilePath(), newItemPath);
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
