//
//  xWebProcessor.h
//  xWebProcessor
//
//  Created by Matthias Müller on 02.05.11.
//  Copyright 2011 Smartkinematics. All rights reserved.
//

#include <iostream>
#include <boost/filesystem.hpp>

#include "model/xWebProc/xWebStringList.h"


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

    void setProjectFilePath(std::string path);

    void addCLIStringParam(const std::string& key, const std::string& value);
    
    bool run();
    
private:
    bool prepareOutputFolder(xWebProcessContext& context);
    bool processContent(xWebProcessContext& context, xWebML::FolderType& root);
    
    bool processFolder(xWebProcessContext& context, xWebML::FolderType& folder);
    bool processStaticFolder(xWebProcessContext& context, xWebML::StaticFolderType& staticFolder);
    bool processFileItem(xWebProcessContext& context, xWebML::FileItemType& fileItem);

    bool copyFolder(boost::filesystem::path sourcefolder, boost::filesystem::path destFolder);
    
private:
    std::string _projectFilePath;
    
    std::auto_ptr<xWebML::ProjectType> _projectFile;

    xWebStringList _cliStrings;
};
