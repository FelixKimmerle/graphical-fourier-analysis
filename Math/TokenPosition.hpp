#pragma once
#include <iostream>
class TokenPosition
{
  public:
    TokenPosition(int, int, int);
    ~TokenPosition();

    int GetColumn();
    int GetIndex();
    int GetLine();
    std::string ToString();

  private:
    int Column;
    int Index;
    int Line;
};
