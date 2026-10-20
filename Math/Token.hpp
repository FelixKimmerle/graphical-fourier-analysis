#pragma once
#include "TokenPosition.hpp"
#include <iostream>
#include <sstream>
using namespace std;
class Token
{
  public:
    Token(int Type, string Value, TokenPosition Position);
    ~Token();

    TokenPosition GetTokenPosition();
    int GetType();
    string GetValue();

    void SetTokenPosition(TokenPosition value);
    void SetType(int value);
    void SetValue(string value);

    string ToString();

  private:
    TokenPosition Position;
    int Type;
    string Value;
};
