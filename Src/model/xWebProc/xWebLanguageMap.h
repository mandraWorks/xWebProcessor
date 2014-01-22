#ifndef XWEBLANGUAGEMAP_H
#define XWEBLANGUAGEMAP_H

#include <string>
#include <vector>
#include <map>
#include <boost/filesystem.hpp>

class xWebStringList;

class xWebLanguageMap
{
public:
    xWebLanguageMap();
    virtual ~xWebLanguageMap();

    void buildLanguageMap(std::string contentFolder);

    int count() const;
    std::string languageKey(int index) const;
    xWebStringList language(const std::string& key) const;

private:
    void buildLanguageEntry(std::string key, boost::filesystem::path languageFolder);

private:
    std::vector<std::string>              _languageKeys;
    std::map<std::string, xWebStringList> _languages;
};

#endif // XWEBLANGUAGEMAP_H
