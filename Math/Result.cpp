#include "Result.hpp"

Result::Result(Complex p_number, bool p_isX) : isX(p_isX), number(p_number)
{
}

void Result::Append(std::vector<Item>* p_Programm) const
{
    if (isX)
    {
        for (auto x = m_Programm.begin(); x != m_Programm.end(); x++)
        {
            p_Programm->push_back(*x);
        }
    }
    else
    {
        p_Programm->push_back(Item(OPUSH));
        p_Programm->push_back(Item(number));
    }
}

void Result::Append(const Result& rhs)
{
    if (rhs.isX)
    {
        isX = true;
        for (auto x = rhs.m_Programm.begin(); x != rhs.m_Programm.end(); x++)
        {
            m_Programm.push_back(*x);
        }
    }
    else
    {
        m_Programm.push_back(Item(OPUSH));
        m_Programm.push_back(Item(rhs.number));
    }
}

void Result::Add(const Item& item)
{
    m_Programm.push_back(item);
}

void Result::operator*=(const Result& rhs)
{
    if (isX)
    {
        rhs.Append(&m_Programm);
        m_Programm.push_back(Item(OMUL));
        return;
    }
    else if (rhs.isX)
    {
        m_Programm.push_back(Item(OPUSH));
        m_Programm.push_back(Item(number));
        rhs.Append(&m_Programm);
        m_Programm.push_back(Item(OMUL));
        isX = true;
        return;
    }
    number *= rhs.number;
}
void Result::operator^=(const Result& rhs)
{
    if (isX)
    {
        rhs.Append(&m_Programm);
        m_Programm.push_back(Item(OPOW));
        return;
    }
    else if (rhs.isX)
    {
        m_Programm.push_back(Item(OPUSH));
        m_Programm.push_back(Item(number));
        rhs.Append(&m_Programm);
        m_Programm.push_back(Item(OPOW));
        isX = true;
        return;
    }
    number = pow(number, rhs.number);
}
void Result::operator/=(const Result& rhs)
{
    if (isX)
    {
        rhs.Append(&m_Programm);
        m_Programm.push_back(Item(ODIV));
        return;
    }
    else if (rhs.isX)
    {
        m_Programm.push_back(Item(OPUSH));
        m_Programm.push_back(Item(number));
        rhs.Append(&m_Programm);
        m_Programm.push_back(Item(ODIV));
        isX = true;
        return;
    }
    number /= rhs.number;
}
void Result::operator+=(const Result& rhs)
{
    if (isX)
    {
        rhs.Append(&m_Programm);
        m_Programm.push_back(Item(OADD));
        return;
    }
    else if (rhs.isX)
    {
        m_Programm.push_back(Item(OPUSH));
        m_Programm.push_back(Item(number));
        rhs.Append(&m_Programm);
        m_Programm.push_back(Item(OADD));
        isX = true;
        return;
    }
    number += rhs.number;
}
void Result::operator-=(const Result& rhs)
{
    if (isX)
    {
        rhs.Append(&m_Programm);
        m_Programm.push_back(Item(OSUB));
        return;
    }
    else if (rhs.isX)
    {
        m_Programm.push_back(Item(OPUSH));
        m_Programm.push_back(Item(number));
        rhs.Append(&m_Programm);
        m_Programm.push_back(Item(OSUB));
        isX = true;
        return;
    }
    number -= rhs.number;
}
void Result::operator=(const Complex& rhs)
{
    isX = false;
    number = rhs;
}
Result Result::operator*(const double& rhs)
{
    if (isX)
    {
        return Result(0.0, true);
    }
    return Result(number * rhs, false);
}

void Result::operator*=(const double& rhs)
{
    number *= rhs;
}