#include "xWebStringsParser.h"
#include "mandraworks/core/log/Log.h"

xWebStringsParser::xWebStringsParser(QString filename) :
  _filename(filename),_currentState(Idle)
{
}

bool xWebStringsParser::parse() {
  QFile file(_filename);

  if ( file.open(QIODevice::ReadOnly) == false ) {
    mandraworks::core::log::Log::error(QString("Could not open file: %1").arg(_filename));
    return false;
  }

  QTextStream stream(&file);

  while ( stream.atEnd() == false ) {
    _currentChar = stream.read(1);

    if ( _currentChar.compare("\"") == 0 )
      processQuotes();
    else if ( _currentChar.compare("=") == 0 )
      processAssignment();
    else if ( _currentChar.compare(";") == 0 )
      processSemicolon();
    else
      processChar();

    if ( _currentState == Error ) {
      mandraworks::core::log::Log::error(QString("Parser error: %1 %2").arg(_currentKey).arg(_currentValue));
      return false;
    }

    _previousChar = _currentChar;
  }

  return true;
}

void xWebStringsParser::processQuotes() {
  if ( _currentState == Idle ) {
    _currentState = InKey;
  }
  else if ( _currentState == InKey ) {
    _currentState = BeforeAssignemet;
  }
  else if ( _currentState == AfterAssignemet ) {
    _currentState = InValue;
  }
  else if ( _currentState == InValue ) {
    if ( _previousChar.compare("\\") != 0 ) //ignore \"
      _currentState = RowFinished;
    else
      _currentValue += _currentChar;
  }
}

void xWebStringsParser::processAssignment() {
  if ( _currentState == BeforeAssignemet ) {
    _currentState = AfterAssignemet;
  }
}

void xWebStringsParser::processSemicolon() {
  if ( _currentState == RowFinished ) {
    _data.insert(_currentKey, _currentValue);
    _currentKey.clear();
    _currentValue.clear();

    _currentState = Idle;
  }
  else
    _currentValue += _currentChar;
}

void xWebStringsParser::processChar() {
  if ( _currentState == InKey )
    _currentKey += _currentChar;
  else if ( _currentState == InValue )
    _currentValue += _currentChar;
}
