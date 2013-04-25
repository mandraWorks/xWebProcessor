#ifndef XWEBSTRINGSPARSER_H
#define XWEBSTRINGSPARSER_H

#include <QtCore>


class xWebStringsParser
{
public:
  xWebStringsParser(QString filename);

  bool parse();

  const QMap<QString,QString>* data() const { return &_data; }

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

  QString    _filename;

  ParseState _currentState;
  QString    _previousChar;
  QString    _currentChar;

  QString    _currentKey;
  QString    _currentValue;

  QMap<QString,QString> _data;
};

#endif // XWEBSTRINGSPARSER_H
