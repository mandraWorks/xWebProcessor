#include "xWebLanguageMap.h"


#include "model/xWebProc/xWebStringList.h"

xWebLanguageMap::xWebLanguageMap()
{
}

xWebLanguageMap::~xWebLanguageMap()
{

}

void xWebLanguageMap::buildLanguageMap(std::string contentFolder)
{
    _languageKeys.clear();
    _languages.clear();

    boost::filesystem::path contentFolderPath = contentFolder;

    if ( boost::filesystem::exists(contentFolderPath) == false )
    {
        std::cout << "ERROR: Contentfolder do not exists: " << contentFolder << std::endl;
        return;
    }

    boost::filesystem::directory_iterator end_it;
    for ( boost::filesystem::directory_iterator it(contentFolderPath);
          it != end_it; it++)
    {
        if ( boost::filesystem::is_directory(it->status()) == true )
        {
            boost::filesystem::path languageFolder = *it;
            std::string languageKey = languageFolder.filename().string();
            buildLanguageEntry(languageKey, languageFolder);
        }
    }
}

int xWebLanguageMap::count() const
{
    return _languageKeys.size();
}

std::string xWebLanguageMap::languageKey(int index) const
{
    return _languageKeys[index];
}

xWebStringList xWebLanguageMap::language(const std::string &key) const
{
    return _languages.find(key)->second;
}

void xWebLanguageMap::buildLanguageEntry(std::string key, boost::filesystem::path languageFolder)
{
    _languageKeys.push_back(key);

    xWebStringList language;

    boost::filesystem::directory_iterator end_it;
    for ( boost::filesystem::directory_iterator it(languageFolder);
          it != end_it; it++)
    {
        boost::filesystem::path entry = *it;
        if ( boost::filesystem::is_regular_file(entry) == true )
        {
            language.appendContentFile( entry.string(), true);
        }
    }

    _languages.insert(std::pair<std::string, xWebStringList>( key, language));
}
