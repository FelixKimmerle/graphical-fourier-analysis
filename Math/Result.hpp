#pragma once
#include <complex>
#include <iostream>
#include <vector>
typedef std::complex<double> Complex;

enum Operation
{
    OPUSH,
    OADD,
    OSUB,
    OMUL,
    ODIV,
    OPOW,
    OSIN,
    OASIN,
    OCOS,
    OACOS,
    OSQRT,
    OPUSHX,
    OTAN,
    OATAN,
    OPLOG,
    OLOG,
    ONEG,
    OImag,
    OReal,
    OABS,
};

union Item {
    Item(Complex p_number) : number(p_number){};
    Item(Operation p_op) : op(p_op){};
    Complex number;
    Operation op;
};

class Result
{
  private:
    bool isX;
    Complex number;
    std::vector<Item> m_Programm;

  public:
    Result(Complex p_number, bool p_isX = false);
    void Append(std::vector<Item> *p_Programm) const;
    void Append(const Result & rhs);
    void Add(const Item &item);
    std::vector<Item> GetProgramm()
    {
        if (!isX)
        {
            m_Programm.push_back(Item(OPUSH));
            m_Programm.push_back(Item(number));
        }
        return m_Programm;
    }
    bool HasX()
    {
        return isX;
    }
    void setX()
    {
        isX = true;
    }
    Complex GetNumber()
    {
        if (isX)
        {
            std::cout << "Warning!" << std::endl;
        }
        return number;
    }

    void operator*=(const Result &rhs);
    void operator^=(const Result &rhs);
    void operator*=(const double &rhs);
    void operator/=(const Result &rhs);
    void operator+=(const Result &rhs);
    void operator-=(const Result &rhs);
    void operator=(const Complex &rhs);
    Result operator*(const double &rhs);
};
