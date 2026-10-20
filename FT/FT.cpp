#include "FT.hpp"
#include <cmath>

FT::FT(const std::vector<float> &p_Data, float p_fStartFrequency)
    : m_Data(p_Data), m_Positions(p_Data.size()), m_fStartFrequency(p_fStartFrequency),
      m_fFrequency(p_fStartFrequency)
{
    // Normalize the signal into [0, 1] so that 0 maps onto the inner circle (radius 0.5)
    float fMax = 0;
    for (auto &&i : m_Data)
    {
        fMax = std::max(fMax, std::abs(i));
    }
    if (fMax == 0)
    {
        fMax = 1;
    }
    fMax *= 2;
    for (auto &&i : m_Data)
    {
        i /= fMax;
        i += 0.5f;
    }
}

std::complex<float> FT::Update(float p_fStep)
{
    m_fFrequency += p_fStep;
    std::complex<float> sum(0, 0);
    if (m_Data.empty())
    {
        return sum;
    }
    const float fact = m_fFrequency * 2.f * static_cast<float>(M_PI) / m_Data.size();
    for (size_t c = 0; c < m_Data.size(); c++)
    {
        const float angle = (c + 1) * fact;
        std::complex<float> point(m_Data[c] * std::sin(angle), m_Data[c] * std::cos(angle));
        m_Positions[c] = point;
        sum += point;
    }
    sum /= static_cast<float>(m_Positions.size());
    return sum;
}

void FT::Reset()
{
    m_fFrequency = m_fStartFrequency;
}

float FT::GetFrequency() const
{
    return m_fFrequency;
}

const std::vector<std::complex<float>> &FT::GetData() const
{
    return m_Positions;
}
