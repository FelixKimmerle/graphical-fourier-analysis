#include "Token.hpp"

Token::Token(int type, string value, TokenPosition position) : Position(position), Type(type), Value(value)
{
}

Token::~Token()
{
}

TokenPosition Token::GetTokenPosition()
{
    return Position;
}

int Token::GetType()
{
    return Type;
}

string Token::GetValue()
{
    return Value;
}

void Token::SetTokenPosition(TokenPosition value)
{
    Position = value;
}

void Token::SetType(int value)
{
    Type = value;
}

void Token::SetValue(string value)
{
    Value = value;
}

string Token::ToString()
{
    return "Token: {{ Type: \"" + std::to_string(Type) + "\", Value: \"" + Value + "\", Position: {{ Index: \"" +
           std::to_string(Position.GetIndex()) + "\", Line: \"" + std::to_string(Position.GetLine()) +
           "\", Column: \"" + std::to_string(Position.GetColumn()) + "\" }} }}";
}
