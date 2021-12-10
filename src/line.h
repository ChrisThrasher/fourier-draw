#pragma once

#include <SFML/Graphics.hpp>

class Line : public sf::Drawable {
    std::vector<sf::Vector2f> m_vertices {};
    sf::Color m_color { sf::Color::White };

public:
    void draw(sf::RenderTarget& target, sf::RenderStates) const override;

    void PushBack(const sf::Vector2f& vertex);
    void SetBrightness(const sf::Uint8 brightness);
    auto Size() const { return m_vertices.size(); }
    auto Empty() const { return m_vertices.empty(); }
    auto Back() const { return m_vertices.back(); }
    auto At(const size_t i) const { return m_vertices.at(i); }
};
