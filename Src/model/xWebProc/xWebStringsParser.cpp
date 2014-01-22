#include <iostream>
#include <fstream>
#include "xWebStringsParser.h"

xWebStringsParser::xWebStringsParser(std::string filename) :
    _filename(filename),_currentState(Idle)
{
}

void xWebStringsParser::setKeyPrefix(std::string key)
{
    _keyPrefix = key;
}

bool xWebStringsParser::parse() {
    std::ifstream file;
    file.open(_filename.c_str(), std::ios::in);

    if ( !file ) {
        std::cout << "Could not open file: " << _filename << std::endl;
        return false;
    }

    while ( !file.eof() ) {
        file.get(_currentChar);

        if ( _currentChar == '\"' )
            processQuotes();
        else if ( _currentChar == '=' )
            processAssignment();
        else if ( _currentChar == ';' )
            processSemicolon();
        else
            processChar();

        if ( _currentState == Error ) {
            std::cout << "Parser error: " << _currentKey << _currentValue << std::endl;
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
    else if(_currentState == InValue)
    {
        _currentValue += _currentChar;
    }
}

void xWebStringsParser::processSemicolon() {
    if ( _currentState == RowFinished ) {
        std::string key;
        if ( _keyPrefix.length() == 0 )
            key = _currentKey;
        else
            key = _keyPrefix + "_" + _currentKey;
        _data.insert(std::pair<std::string,std::string>(key, _currentValue));
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
