#include "RoundChart.hpp"

RoundChart::RoundChart(float p_fRadius) : m_Line(sf::PrimitiveType::LineStrip), m_fRadius(p_fRadius)
{
    m_OneShape.setPointCount(100);
    m_OneShape.setOutlineThickness(-2);
    m_OneShape.setOutlineColor(sf::Color::White);
    m_OneShape.setFillColor(sf::Color(0, 255, 0, 20));

    m_ZeroShape.setPointCount(100);
    m_ZeroShape.setOutlineThickness(-2);
    m_ZeroShape.setOutlineColor(sf::Color::White);
    m_ZeroShape.setFillColor(sf::Color(0, 0, 255, 20));

    m_ZeroShapeBackground.setPointCount(100);
    m_ZeroShapeBackground.setFillColor(sf::Color::Black);

    m_Center.setFillColor(sf::Color::Cyan);

    SetRadius(p_fRadius);
}

void RoundChart::SetRadius(float p_fRadius)
{
    m_fRadius = p_fRadius;

    m_OneShape.setRadius(p_fRadius + 1);
    m_OneShape.setOrigin({p_fRadius + 1, p_fRadius + 1});

    m_ZeroShape.setRadius(p_fRadius / 2 + 1);
    m_ZeroShape.setOrigin({p_fRadius / 2 + 1, p_fRadius / 2 + 1});

    m_ZeroShapeBackground.setRadius(p_fRadius / 2 + 1);
    m_ZeroShapeBackground.setOrigin({p_fRadius / 2 + 1, p_fRadius / 2 + 1});

    m_Center.setRadius(p_fRadius / 50.f);
    m_Center.setOrigin({m_Center.getRadius(), m_Center.getRadius()});
}

void RoundChart::Update(const std::vector<std::complex<float>>& p_vData, std::complex<float> center)
{
    m_Line.resize(p_vData.size());
    for (size_t i = 0; i < p_vData.size(); i++)
    {
        m_Line[i].position = {p_vData[i].real() * m_fRadius, p_vData[i].imag() * m_fRadius};
        m_Line[i].color = sf::Color::Yellow;
    }
    m_Center.setPosition({center.real() * m_fRadius, center.imag() * m_fRadius});
}

void RoundChart::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();

    target.draw(m_OneShape, states);
    target.draw(m_ZeroShapeBackground, states);
    target.draw(m_ZeroShape, states);
    target.draw(m_Line, states);
    target.draw(m_Center, states);
}
