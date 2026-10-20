#pragma once
#include <SFML/Graphics.hpp>
#include <deque>

// Scrolling line plot that keeps the last p_uiCapacity values and scales itself to the largest value seen.
class RectanglePlot : public sf::Drawable, public sf::Transformable
{
private:
    sf::RectangleShape m_ZeroLine;
    sf::RectangleShape m_Outline;
    std::deque<float> m_Data;
    float m_fMax;
    unsigned int m_uiCapacity;

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

public:
    RectanglePlot(sf::Vector2f p_Size, unsigned int p_uiCapacity = 1000);
    void SetSize(sf::Vector2f p_Size);
    void Add(float p_fValue);
    void Clear();
};
