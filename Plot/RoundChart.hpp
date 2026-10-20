#pragma once
#include <SFML/Graphics.hpp>
#include <complex>
#include <vector>

// Shows the signal wound around a circle together with its center of mass.
class RoundChart : public sf::Drawable, public sf::Transformable
{
  private:
    sf::VertexArray m_Line;
    sf::CircleShape m_ZeroShape;
    sf::CircleShape m_ZeroShapeBackground;
    sf::CircleShape m_OneShape;
    sf::CircleShape m_Center;
    float m_fRadius;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

  public:
    RoundChart(float p_fRadius = 100.f);
    void SetRadius(float p_fRadius);
    void Update(const std::vector<std::complex<float>>& p_vData, std::complex<float> center);
};
