//
//  xWebFileItemProcessor.h
//  xWebProcessor
//
//  Created by Matthias Müller on 05.05.11.
//  Copyright 2011 Smartkinematics. All rights reserved.
//

#include <string>
#include <list>

namespace xWebML {
    class FileItemType;
}
class xWebProcessContext;
class xWebStringList;


class xWebFileItemProcessor {
  
public:
    xWebFileItemProcessor(xWebML::FileItemType& fileItem);
    ~xWebFileItemProcessor();
    
    bool run(xWebProcessContext& context);
    
    bool runTransform(xWebProcessContext& context);
    
private:
    void initRun(xWebProcessContext& context);
    void releaseRun(xWebProcessContext& context);
    
private:
    std::string _processingMethod;
    std::string _sourceFilePath;
    std::string _outputFolder;
    std::string _targetFileName;
    std::string _contentPrefix;
    std::list<std::string>  _activeMenuIDs;
    
    xWebStringList* _strings;
};
