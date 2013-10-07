#ifndef XWEBSTRINGSPARSER_H
#define XWEBSTRINGSPARSER_H

#include <string>
#include <map>


class xWebStringsParser
{
public:
  xWebStringsParser(std::string filename);

  bool parse();

  const std::map<std::string,std::string>* data() const { return &_data; }

private:
  void processQuotes();
  void processAssignment();
  void processSemicolon();
  void processChar();

private:
  enum ParseState {
    Idle,
    InKey,
    BeforeAssignemet,
    AfterAssignemet,
    InValue,
    RowFinished,
    Error
  };

  std::string    _filename;

  ParseState _currentState;
  std::string    _previousChar;
  char           _currentChar;

  std::string    _currentKey;
  std::string    _currentValue;

  std::map<std::string,std::string> _data;
};

#endif // XWEBSTRINGSPARSER_H
