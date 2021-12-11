#pragma once

#include <SFML/Graphics.hpp>

class Line : public sf::Drawable {
    std::vector<sf::Vector2f> m_vertices {};
    sf::Color m_color { sf::Color::White };

public:
    void draw(sf::RenderTarget& target, sf::RenderStates) const override;

    void push_back(const sf::Vector2f& vertex);
    void set_brightness(const sf::Uint8 brightness);
    [[nodiscard]] auto size() const { return m_vertices.size(); }
    [[nodiscard]] auto empty() const { return m_vertices.empty(); }
    [[nodiscard]] auto back() const { return m_vertices.back(); }
    [[nodiscard]] auto at(const size_t i) const { return m_vertices.at(i); }
};
