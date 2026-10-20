#pragma once
#include "Token.hpp"
#include "TokenDefinition.hpp"
#include <iostream>
#include <list>
#include <regex>
using namespace std;
class Lexer
{
  public:
    Lexer();
    ~Lexer();

    void AddDefinition(TokenDefinition);
    void AddDefinition(int numbrt, string reg, bool ignore = false);
    list<Token> Tokenize(string source);

  private:
    regex endOfLineRegex;
    list<TokenDefinition> tokenDefinitions;
};
