//
//  xWebFileItemProcessor.h
//  xWebProcessor
//
//  Created by Matthias Müller on 05.05.11.
//  Copyright 2011 Smartkinematics. All rights reserved.
//

#include <QtCore>

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
    QString _processingMethod;
    QString _sourceFilePath;
    QString _outputFolder;
    QString _targetFileName;
    QString _contentPrefix;
    QStringList  _activeMenuIDs;
    
    xWebStringList* _strings;
};
