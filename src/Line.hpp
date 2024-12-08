#pragma once

#include <SFML/Graphics.hpp>

class Line : public sf::Drawable {
    std::vector<sf::Vector2f> m_vertices;
    sf::Color m_color { sf::Color::White };

public:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void push_back(sf::Vector2f vertex);
    void set_brightness(uint8_t brightness);
    [[nodiscard]] auto size() const { return m_vertices.size(); }
    [[nodiscard]] auto empty() const { return m_vertices.empty(); }
    [[nodiscard]] auto back() const { return m_vertices.back(); }
    [[nodiscard]] auto at(const std::size_t i) const { return m_vertices.at(i); }
};
