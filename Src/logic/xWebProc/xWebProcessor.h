//
//  xWebProcessor.h
//  xWebProcessor
//
//  Created by Matthias Müller on 02.05.11.
//  Copyright 2011 Smartkinematics. All rights reserved.
//

#include <iostream>
#include <QtCore>


namespace xWebML {
    class ProjectType; 
    class FileItemType;
    class FolderType;
    class StaticFolderType;
}
class xWebProcessContext;


class xWebProcessor {
  
public:
    xWebProcessor();
    ~xWebProcessor();
    
    void setProjectFilePath(QString path);
    void setProjectFilePath(std::string path);
    
    bool run();
    
private:
    bool prepareOutputFolder(xWebProcessContext& context);
    bool processContent(xWebProcessContext& context, xWebML::FolderType& root);
    
    bool processFolder(xWebProcessContext& context, xWebML::FolderType& folder);
    bool processStaticFolder(xWebProcessContext& context, xWebML::StaticFolderType& staticFolder);
    bool processStaticFolder(xWebProcessContext& context, QString sourceFolder, QString outputFolder);
    bool processFileItem(xWebProcessContext& context, xWebML::FileItemType& fileItem);
    
private:
    QString _projectFilePath;
    
    std::auto_ptr<xWebML::ProjectType> _projectFile;
};
