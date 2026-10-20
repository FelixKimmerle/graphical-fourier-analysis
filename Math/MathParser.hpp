#pragma once
#include "Lexer.hpp"
#include <complex>
#include <map>
#include "Programm.hpp"

class MathParser
{
  public:
	MathParser();
	~MathParser();
	void Parse(string Expression);
	Programm Calc();
	bool HasError() const;

  private:
	enum ETokens
	{
		Error,
		End,
		Whitespace,
		Comma,
		Number,
		Plus,
		Minus,
		Mul,
		Div,
		BracketOpen,
		BracketClose,
		Abs,
		Sin,
		ASin,
		Cos,
		ACos,
		Tan,
		ATan,
		Sqrt,
		Pow,
		Log,
		Imag,
		Real,
		PLog,
		IPow,
		KomplexNumber,
		Pi,
		Eulernumber,
		Unknown,
		CNumber,
	};
	Lexer m_Clexer;
	size_t m_iCurrentPosition;
	list<Token> m_lTokens;
	list<Token>::iterator m_lTokensIterator;
	Token m_CToken;
	bool m_bError;

	void Accept(ETokens token);
	void GetNextToken();

	void ReportError(const string &message);
	bool StartsOperand();

	Result Level0(); // + -
	Result Level1(); // * / and implicit multiplication
	Result Level2(); // unary sign
	Result Level3(); // ^
	Result Level4(); // numbers, constants, functions, brackets

	vector<Result> GetArguments(int Number);
};
