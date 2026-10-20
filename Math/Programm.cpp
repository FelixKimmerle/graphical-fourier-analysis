#include "Programm.hpp"
#include <algorithm>
Programm::Programm() : m_uiStackSize(0)
{
}

Programm::Programm(std::vector<Item> p_Programm)
{
    SetProgramm(p_Programm);
}

void Programm::SetProgramm(std::vector<Item> p_Programm)
{
    m_Programm = p_Programm;
    CalcStack();
}

void Programm::CalcStack()
{
    unsigned int curr = 0;
    m_uiStackSize = 0;
    for (auto x = m_Programm.begin(); x != m_Programm.end(); x++)
    {
        switch (x->op)
        {
        case OPUSH:
            x++;
            curr++;
            m_uiStackSize = std::max(curr, m_uiStackSize);
            break;
        case OPUSHX:
            curr++;
            m_uiStackSize = std::max(curr, m_uiStackSize);
            break;
        case OMUL:
        case ODIV:
        case OSUB:
        case OADD:
        case OPOW:
        case OPLOG:
            curr--;
            break;
        default:
            break;
        }
    }
}

Programm::~Programm()
{
}

Complex Programm::Calc(Complex input)
{
    if (m_Programm.empty())
    {
        return 0;
    }
    std::vector<Complex>& stack = m_Stack;
    stack.clear();
    stack.reserve(m_uiStackSize);

    Complex a;
    Complex b;

    for (auto x = m_Programm.begin(); x != m_Programm.end(); x++)
    {
        switch (x->op)
        {
        case OPUSH:
            stack.push_back((++x)->number);
            break;
        case OPUSHX:
            stack.push_back(input);
            break;
        case OADD:
            a = stack.back();
            stack.pop_back();
            b = stack.back();
            stack.pop_back();
            stack.push_back(a + b);
            break;
        case OSUB:
            a = stack.back();
            stack.pop_back();
            b = stack.back();
            stack.pop_back();
            stack.push_back(b - a);
            break;
        case OMUL:
            a = stack.back();
            stack.pop_back();
            b = stack.back();
            stack.pop_back();
            stack.push_back(a * b);
            break;
        case ODIV:
            a = stack.back();
            stack.pop_back();
            b = stack.back();
            stack.pop_back();
            stack.push_back(b / a);
            break;
        case OPOW:
            a = stack.back();
            stack.pop_back();
            b = stack.back();
            stack.pop_back();
            stack.push_back(pow(b, a));
            break;
        case OSIN:
            a = stack.back();
            stack.pop_back();
            stack.push_back(sin(a));
            break;
        case OASIN:
            a = stack.back();
            stack.pop_back();
            stack.push_back(asin(a));
            break;
        case OCOS:
            a = stack.back();
            stack.pop_back();
            stack.push_back(cos(a));
            break;
        case OACOS:
            a = stack.back();
            stack.pop_back();
            stack.push_back(acos(a));
            break;
        case OSQRT:
            a = stack.back();
            stack.pop_back();
            stack.push_back(sqrt(a));
            break;
        case OTAN:
            a = stack.back();
            stack.pop_back();
            stack.push_back(tan(a));
            break;
        case OATAN:
            a = stack.back();
            stack.pop_back();
            stack.push_back(atan(a));
            break;
        case OLOG:
            a = stack.back();
            stack.pop_back();
            stack.push_back(log(a));
            break;
        case OPLOG:
            a = stack.back();
            stack.pop_back();
            b = stack.back();
            stack.pop_back();
            stack.push_back(log(a) / log(b));
            break;
        case ONEG:
            a = stack.back();
            stack.pop_back();
            stack.push_back(-a);
            break;
        case OImag:
            a = stack.back();
            stack.pop_back();
            stack.push_back(a.imag());
            break;
        case OReal:
            a = stack.back();
            stack.pop_back();
            stack.push_back(a.real());
            break;
        case OABS:
            a = stack.back();
            stack.pop_back();
            stack.push_back(std::abs(a));
            break;
        }
    }
    return stack.back();
}

void Programm::Print()
{
    std::cout << "++++++++++++++++++" << std::endl;
    for (auto x = m_Programm.begin(); x != m_Programm.end(); x++)
    {
        if (x->op == OPUSH)
        {
            std::cout << "PUSH: " << (++x)->number << std::endl;
        }
        else if (x->op == OPUSHX)
        {
            std::cout << "PUSH: X" << std::endl;
        }
        else
        {
            std::cout << "OP: " << x->op << std::endl;
        }
    }
}