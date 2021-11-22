#pragma once

#include <SFML/Graphics.hpp>

class Line {
    std::vector<sf::Vector2f> m_vertices {};
    sf::Color m_color { sf::Color::White };

public:
    void PushBack(const sf::Vector2f& vertex);
    void SetBrightness(const sf::Uint8 brightness);
    void Draw(sf::RenderWindow& window) const;
    auto Size() const { return m_vertices.size(); }
    auto Empty() const { return m_vertices.empty(); }
    auto Back() const { return m_vertices.back(); }
    auto At(const size_t i) const { return m_vertices.at(i); }
};
