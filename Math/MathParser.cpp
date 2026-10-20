#include "MathParser.hpp"
#include <iostream>
using namespace std;
using namespace std::complex_literals;
MathParser::MathParser() : m_CToken(0, "", TokenPosition(0, 0, 0)), m_bError(false)
{
	m_Clexer.AddDefinition(Unknown, "[a-zA-Z]");
	m_Clexer.AddDefinition(Whitespace, "\\s+", true);
	m_Clexer.AddDefinition(Comma, ",");
	m_Clexer.AddDefinition(Number, "\\d+(\\.\\d*)?|\\.\\d+");
	m_Clexer.AddDefinition(Plus, "\\+");
	m_Clexer.AddDefinition(Minus, "-");
	m_Clexer.AddDefinition(Mul, "\\*");
	m_Clexer.AddDefinition(Div, "/");
	m_Clexer.AddDefinition(BracketOpen, "\\(");
	m_Clexer.AddDefinition(BracketClose, "\\)");
	m_Clexer.AddDefinition(Abs, "\\|");
	m_Clexer.AddDefinition(Sin, "sin");
	m_Clexer.AddDefinition(ASin, "asin");
	m_Clexer.AddDefinition(Cos, "cos");
	m_Clexer.AddDefinition(ACos, "acos");
	m_Clexer.AddDefinition(Tan, "tan");
	m_Clexer.AddDefinition(ATan, "atan");
	m_Clexer.AddDefinition(Log, "log");
	m_Clexer.AddDefinition(PLog, "plog");
	m_Clexer.AddDefinition(Sqrt, "sqrt");
	m_Clexer.AddDefinition(Pow, "pow");
	m_Clexer.AddDefinition(KomplexNumber, "i");
	m_Clexer.AddDefinition(Imag, "imag");
	m_Clexer.AddDefinition(Real, "real");
	m_Clexer.AddDefinition(IPow, "\\^");
	m_Clexer.AddDefinition(Pi, "pi");
	m_Clexer.AddDefinition(Eulernumber, "e");
}

MathParser::~MathParser()
{
}

void MathParser::Parse(string Expression)
{
	m_lTokens = m_Clexer.Tokenize(Expression);
	m_lTokensIterator = m_lTokens.begin();
	m_CToken = *m_lTokensIterator;
}

Programm MathParser::Calc()
{
	m_lTokensIterator = m_lTokens.begin();
	m_CToken = *m_lTokensIterator;
	m_iCurrentPosition = 0;
	m_bError = false;
	Result r = Level0();
	if (m_CToken.GetType() != End)
	{
		ReportError("unexpected '" + m_CToken.GetValue() + "'");
	}

	return Programm(r.GetProgramm());
}

bool MathParser::HasError() const
{
	return m_bError;
}

void MathParser::ReportError(const string &message)
{
	m_bError = true;
	cerr << "Error at column " << m_CToken.GetTokenPosition().GetColumn() + 1 << ": " << message << endl;
}

void MathParser::Accept(ETokens token)
{
	if (m_CToken.GetType() != token)
	{
		static const map<int, string> names = {{BracketOpen, "'('"}, {BracketClose, "')'"}, {Abs, "'|'"}, {Comma, "','"}};
		auto name = names.find(token);
		string found = m_CToken.GetType() == End ? "end of input" : "'" + m_CToken.GetValue() + "'";
		ReportError("expected " + (name != names.end() ? name->second : "token") + " but found " + found);
	}
	GetNextToken();
}

bool MathParser::StartsOperand()
{
	switch (m_CToken.GetType())
	{
	case Number:
	case BracketOpen:
	case Sin:
	case ASin:
	case Cos:
	case ACos:
	case Tan:
	case ATan:
	case Sqrt:
	case Pow:
	case Log:
	case Imag:
	case Real:
	case PLog:
	case KomplexNumber:
	case Pi:
	case Eulernumber:
	case Unknown:
		return true;
	default:
		return false;
	}
}

void MathParser::GetNextToken()
{
	if (m_iCurrentPosition + 1 < m_lTokens.size())
	{
		m_iCurrentPosition++;
		m_CToken = *(++m_lTokensIterator);
	}
}

Result MathParser::Level0()
{
	Result result = Level1();

	while (m_CToken.GetType() == Plus || m_CToken.GetType() == Minus)
	{
		if (m_CToken.GetType() == Plus)
		{
			GetNextToken();
			result += Level1();
		}
		else
		{
			GetNextToken();
			result -= Level1();
		}
	}
	return result;
}

Result MathParser::Level1()
{
	Result result = Level2();

	while (m_CToken.GetType() == Mul || m_CToken.GetType() == Div || StartsOperand())
	{
		if (m_CToken.GetType() == Mul)
		{
			GetNextToken();
			result *= Level2();
		}
		else if (m_CToken.GetType() == Div)
		{
			GetNextToken();
			result /= Level2();
		}
		else
		{
			result *= Level3();
		}
	}

	return result;
}

Result MathParser::Level2()
{
	bool sign = false;
	if (m_CToken.GetType() == Minus || m_CToken.GetType() == Plus)
	{
		sign = m_CToken.GetType() == Minus;
		GetNextToken();
	}
	Result result = Level3();
	if (sign)
	{
		if (result.HasX())
		{
			result.Add(ONEG);
		}
		else
		{
			result *= -1;
		}
	}
	return result;
}

Result MathParser::Level3()
{
	Result result = Level4();
	if (m_CToken.GetType() == IPow)
	{
		GetNextToken();
		result ^= Level2();
	}
	return result;
}

Result MathParser::Level4()
{
	Result result(0, 0);
	switch (m_CToken.GetType())
	{
	case Number:
		result = stod(m_CToken.GetValue());
		GetNextToken();
		break;
	case BracketOpen:
		Accept(BracketOpen);
		result = Level0();
		Accept(BracketClose);
		break;
	case Abs:
		Accept(Abs);
		result = Level0();
		Accept(Abs);
		if(!result.HasX())
		{
			result = abs(result.GetNumber());
		}
		else
		{
			result.Add(Item(Operation::OABS));
		}
		break;
	case Sin:
	{
		GetNextToken();
		Accept(BracketOpen);
		Result presult = Level0();
		Accept(BracketClose);
		if (!presult.HasX())
		{
			result = sin(presult.GetNumber());
		}
		else
		{
			presult.Add(Item(Operation::OSIN));
			result = presult;
		}
	}
	break;
	case ASin:
	{
		GetNextToken();
		Accept(BracketOpen);
		Result presult = Level0();
		Accept(BracketClose);
		if (!presult.HasX())
		{
			result = asin(presult.GetNumber());
		}
		else
		{

			presult.Add(Item(Operation::OASIN));
			result = presult;
		}
	}
	break;
	case Cos:
	{
		GetNextToken();
		Accept(BracketOpen);
		Result presult = Level0();
		Accept(BracketClose);
		if (!presult.HasX())
		{
			result = cos(presult.GetNumber());
		}
		else
		{

			presult.Add(Item(Operation::OCOS));
			result = presult;
		}
	}
	break;
	case ACos:
	{
		GetNextToken();
		Accept(BracketOpen);
		Result presult = Level0();
		Accept(BracketClose);
		if (!presult.HasX())
		{
			result = acos(presult.GetNumber());
		}
		else
		{
			presult.Add(Item(Operation::OACOS));
			result = presult;
		}
	}
	break;
	case Tan:
	{
		GetNextToken();
		Accept(BracketOpen);
		Result presult = Level0();
		Accept(BracketClose);
		if (!presult.HasX())
		{
			result = tan(presult.GetNumber());
		}
		else
		{

			presult.Add(Item(Operation::OTAN));
			result = presult;
		}
	}
	break;
	case ATan:
	{
		GetNextToken();
		Accept(BracketOpen);
		Result presult = Level0();
		Accept(BracketClose);
		if (!presult.HasX())
		{
			result = atan(presult.GetNumber());
		}
		else
		{

			presult.Add(Item(Operation::OATAN));
			result = presult;
		}
	}
	break;
	case Sqrt:
	{
		GetNextToken();
		Accept(BracketOpen);
		Result presult = Level0();
		Accept(BracketClose);
		if (!presult.HasX())
		{
			result = sqrt(presult.GetNumber());
		}
		else
		{

			presult.Add(Item(Operation::OSQRT));
			result = presult;
		}
	}
	break;
	case Pow:
	{
		GetNextToken();
		vector<Result> Args = GetArguments(2);
		Result a = Args[0];
		Result b = Args[1];

		if (!a.HasX() && !b.HasX())
		{
			result = pow(a.GetNumber(), b.GetNumber());
		}
		else
		{
			result.Append(a);
			result.Append(b);
			result.Add(Item(Operation::OPOW));
		}
	}
	break;
	case PLog:
	{
		GetNextToken();
		vector<Result> Args = GetArguments(2);
		Result a = Args[0];
		Result b = Args[1];

		if (!a.HasX() && !b.HasX())
		{
			result = log(b.GetNumber()) / log(a.GetNumber());
		}
		else
		{
			result.Append(a);
			result.Append(b);
			result.Add(Item(Operation::OPLOG));
		}
	}
	break;
	case Log:
	{
		GetNextToken();
		Accept(BracketOpen);
		Result presult = Level0();
		Accept(BracketClose);
		if (!presult.HasX())
		{
			result = log(presult.GetNumber());
		}
		else
		{

			presult.Add(Item(Operation::OLOG));
			result = presult;
		}
	}
	break;
	case Imag:
	{
		GetNextToken();
		Accept(BracketOpen);
		Result presult = Level0();
		Accept(BracketClose);
		if (!presult.HasX())
		{
			result = (presult.GetNumber()).imag();
		}
		else
		{

			presult.Add(Item(Operation::OImag));
			result = presult;
		}
	}
	break;
	case Real:
	{
		GetNextToken();
		Accept(BracketOpen);
		Result presult = Level0();
		Accept(BracketClose);
		if (!presult.HasX())
		{
			result = (presult.GetNumber()).real();
		}
		else
		{

			presult.Add(Item(Operation::OReal));
			result = presult;
		}
	}
	break;
	case Pi:
		result = 3.141592653589793238462643383279502884L;
		GetNextToken();
		break;
	case Eulernumber:
		result = 2.7182818284590452353602874713526625L;
		GetNextToken();
		break;
	case KomplexNumber:
		result = 1i;
		GetNextToken();
		break;
	case Unknown:
		result.Add(Operation::OPUSHX);
		result.setX();
		GetNextToken();
		break;
	case End:
		ReportError("unexpected end of input");
		break;
	default:
		ReportError("unexpected '" + m_CToken.GetValue() + "'");
		GetNextToken();
		break;
	}
	return result;
}

vector<Result> MathParser::GetArguments(int Number)
{
	Accept(BracketOpen);
	vector<Result> Arguments;
	int Counter = 1;
	Arguments.push_back(Level0());
	while (m_CToken.GetType() == Comma && m_CToken.GetType() != End)
	{
		Accept(Comma);
		if (Counter >= Number)
		{
			ReportError("too many arguments");
			break;
		}
		Arguments.push_back(Level0());
		Counter++;
	}
	if (Number > Counter)
	{
		ReportError("not enough arguments");
		Arguments.resize(Number, Result(0));
	}
	Accept(BracketClose);
	return Arguments;
}
