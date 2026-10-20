#include "FT/FT.hpp"
#include "Math/MathParser.hpp"
#include "Plot/RectanglePlot.hpp"
#include "Plot/RoundChart.hpp"
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <cstdio>
#include <iostream>

namespace
{
constexpr unsigned int SAMPLES = 1000;   // number of samples taken from f(x)
constexpr float SAMPLE_STEP = 0.01f;     // distance between samples, so x runs from 0 to 10
constexpr float FREQUENCY_STEP = 0.006f; // winding frequency increase per frame (at speed 1)
constexpr float MARGIN = 30.f;

void Layout(sf::Vector2u size, RoundChart& chart, RectanglePlot& plot)
{
    const float width = static_cast<float>(size.x);
    const float height = static_cast<float>(size.y);
    const float plotHeight = std::max(80.f, height * 0.22f);
    const float radius = std::max(10.f, std::min(width - 2 * MARGIN, height - plotHeight - 3 * MARGIN) / 2);

    chart.SetRadius(radius);
    chart.setPosition({width / 2, MARGIN + radius});

    plot.setPosition({MARGIN, 2 * MARGIN + 2 * radius});
    plot.SetSize({width - 2 * MARGIN, height - 3 * MARGIN - 2 * radius});
}
} // namespace

int main(int argc, char** argv)
{
    std::string sSource;
    if (argc == 1)
    {
        std::cout << "f(x) = ";
        std::getline(std::cin, sSource);
    }
    else
    {
        for (int i = 1; i < argc; i++)
        {
            sSource += (i > 1 ? " " : "") + std::string(argv[i]);
        }
        std::cout << "f(x) = " << sSource << std::endl;
    }

    MathParser mp;
    mp.Parse(sSource);
    auto programm = mp.Calc();
    if (mp.HasError())
    {
        return EXIT_FAILURE;
    }

    std::vector<float> data(SAMPLES);
    for (size_t i = 0; i < data.size(); i++)
    {
        data[i] = static_cast<float>(programm.Calc(i * SAMPLE_STEP).real());
    }
    FT ft(data);

    sf::ContextSettings settings;
    settings.antiAliasingLevel = 4;
    sf::RenderWindow window(sf::VideoMode({900, 1000}), "Graphical Fourier Analysis", sf::Style::Default,
                            sf::State::Windowed, settings);
    window.setVerticalSyncEnabled(true);

    RoundChart chart;
    RectanglePlot plot({1, 1});
    Layout(window.getSize(), chart, plot);

    bool bPaused = false;
    float fSpeed = 1.f;
    unsigned int uiFrame = 0;
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            else if (const auto* resized = event->getIf<sf::Event::Resized>())
            {
                window.setView(sf::View(sf::FloatRect({0, 0}, sf::Vector2f(resized->size))));
                Layout(resized->size, chart, plot);
            }
            else if (const auto* key = event->getIf<sf::Event::KeyPressed>())
            {
                switch (key->code)
                {
                case sf::Keyboard::Key::Escape:
                    window.close();
                    break;
                case sf::Keyboard::Key::Space:
                    bPaused = !bPaused;
                    break;
                case sf::Keyboard::Key::R:
                    ft.Reset();
                    plot.Clear();
                    break;
                case sf::Keyboard::Key::Up:
                    fSpeed = std::min(fSpeed * 2, 32.f);
                    break;
                case sf::Keyboard::Key::Down:
                    fSpeed = std::max(fSpeed / 2, 1.f / 32);
                    break;
                default:
                    break;
                }
            }
        }

        if (!bPaused)
        {
            auto center = ft.Update(FREQUENCY_STEP * fSpeed);
            plot.Add(center.real());
            chart.Update(ft.GetData(), center);
        }

        if (uiFrame++ % 10 == 0)
        {
            // Frequency in windings per unit of x (the samples span SAMPLES * SAMPLE_STEP units)
            char title[128];
            std::snprintf(title, sizeof(title), "Graphical Fourier Analysis - %.3f windings per unit - speed x%g%s",
                          ft.GetFrequency() / (SAMPLES * SAMPLE_STEP), fSpeed, bPaused ? " - paused" : "");
            window.setTitle(title);
        }

        window.clear();
        window.draw(chart);
        window.draw(plot);
        window.display();
    }

    return EXIT_SUCCESS;
}
