#include "TokenPosition.hpp"
#include <string>
TokenPosition::TokenPosition(int Index, int Line, int Column)
{
    this->Column = Column;
    this->Index = Index;
    this->Line = Line;
}

TokenPosition::~TokenPosition()
{
}

int TokenPosition::GetColumn()
{
    return Column;
}

int TokenPosition::GetIndex()
{
    return Index;
}

int TokenPosition::GetLine()
{
    return Line;
}

std::string TokenPosition::ToString()
{
    return "Line: " + std::to_string(Line) + " Column: " + std::to_string(Column) + " Index: " + std::to_string(Index);
}
