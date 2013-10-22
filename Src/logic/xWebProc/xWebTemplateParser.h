//
//  xWebTemplateParser.h
//  xWebProcessor
//
//  Created by Matthias Müller on 06.05.11.
//  Copyright 2011 Smartkinematics. All rights reserved.
//
#ifndef XWEBPROCESSOR_HEADER
#define XWEBPROCESSOR_HEADER

#include <string>
#include <list>
#include <fstream>


class xWebTemplateParserDelegate {
public:
  virtual void processString(std::string xmlData, std::ofstream& outStream) = 0;
  virtual void processContentLink(std::string xmlData, std::ofstream& outStream) = 0;
  virtual void processContent(std::string xmlData, std::ofstream& outStream) = 0;
  virtual void processMenu(std::string xmlData, std::ofstream& outStream) = 0;
  virtual void processIncludeFile(std::string xmlData, std::ofstream& outStream) = 0;
};


class xWebTemplateParser {

  enum Parser_State {
    Idle,
    InTag
  };

public:
  xWebTemplateParser();
  xWebTemplateParser(std::string inputFilePath, std::string outputFilePath);
  ~xWebTemplateParser();

  void setDelegate(xWebTemplateParserDelegate* delegate) { _delegate = delegate; }

  void init();
  bool run();
  void parseLine(std::string line, std::ofstream& outStream);

private:
  void processCurrentElement(std::ofstream& outStream);

  std::string getStartTag(std::string tag);
  std::string getEndTag(std::string tag);

private:
  std::string _inputFilePath;
  std::string _outputFilePath;
  std::list<std::string> _xWebTags;

  int _parserState;
  std::list<std::string>::iterator _currentTagIndex;

  std::string _currentElement;

  xWebTemplateParserDelegate* _delegate;
};

#endif
