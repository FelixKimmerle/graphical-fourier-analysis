#include "RectanglePlot.hpp"
#include <cmath>

RectanglePlot::RectanglePlot(sf::Vector2f p_Size, unsigned int p_uiCapacity) : m_fMax(0.f), m_uiCapacity(p_uiCapacity)
{
    m_ZeroLine.setFillColor(sf::Color(128, 128, 128));

    m_Outline.setOutlineThickness(1);
    m_Outline.setOutlineColor(sf::Color::Magenta);
    m_Outline.setFillColor(sf::Color::Transparent);

    SetSize(p_Size);
}

void RectanglePlot::SetSize(sf::Vector2f p_Size)
{
    m_Outline.setSize(p_Size);
    m_ZeroLine.setSize({p_Size.x, 1});
    m_ZeroLine.setPosition({0, p_Size.y / 2});
}

void RectanglePlot::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(m_ZeroLine, states);

    const sf::Vector2f size = m_Outline.getSize();
    const float fStep = m_uiCapacity > 1 ? size.x / (m_uiCapacity - 1) : 0.f;
    const float fScale = m_fMax > 0 ? size.y / (m_fMax * 2) : 0.f;

    sf::VertexArray line(sf::PrimitiveType::LineStrip, m_Data.size());
    for (size_t c = 0; c < m_Data.size(); c++)
    {
        line[c].position = {c * fStep, size.y / 2 - m_Data[c] * fScale};
        line[c].color = sf::Color::Green;
    }
    target.draw(line, states);
    target.draw(m_Outline, states);
}

void RectanglePlot::Add(float p_fValue)
{
    if (m_Data.size() >= m_uiCapacity)
    {
        m_Data.pop_front();
    }
    m_Data.push_back(p_fValue);
    m_fMax = std::max(m_fMax, std::abs(p_fValue));
}

void RectanglePlot::Clear()
{
    m_Data.clear();
    m_fMax = 0.f;
}
