#pragma once
#include <complex>
#include <vector>

// Winds a sampled signal around the origin ("almost Fourier transform" from 3Blue1Brown)
// and tracks the center of mass of the wound-up graph.
class FT
{
private:
    std::vector<float> m_Data;
    std::vector<std::complex<float>> m_Positions;
    float m_fStartFrequency;
    float m_fFrequency;

public:
    FT(const std::vector<float> &p_Data, float p_fStartFrequency = 1.f);
    std::complex<float> Update(float p_fStep);
    void Reset();
    float GetFrequency() const;
    const std::vector<std::complex<float>> &GetData() const;
};
