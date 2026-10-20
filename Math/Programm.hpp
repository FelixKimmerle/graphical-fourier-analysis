#pragma once
#include "Result.hpp"

class Programm
{
  private:
    unsigned int m_uiStackSize;
    std::vector<Item> m_Programm;
    std::vector<Complex> m_Stack;

    void CalcStack();

  public:
    Programm(std::vector<Item> p_Programm);
    Programm();
    void SetProgramm(std::vector<Item> p_Programm);
    ~Programm();
    void Print();
    Complex Calc(Complex input);
};
