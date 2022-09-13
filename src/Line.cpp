#include "Line.hpp"

#include <cmath>

void Line::push_back(const sf::Vector2f& vertex) { m_vertices.push_back(vertex); }

void Line::set_brightness(const uint8_t brightness) { m_color = { brightness, brightness, brightness }; }

void Line::draw(sf::RenderTarget& target, const sf::RenderStates& /* states */) const
{
    constexpr auto stroke = 5.0f;

    // Draw line segments
    for (size_t i = 0; i + 1 < m_vertices.size(); ++i) {
        const auto segment = m_vertices[i + 1] - m_vertices[i];

        auto line = sf::RectangleShape({ segment.length(), stroke });
        line.setOrigin({ 0.0f, stroke / 2.0f });
        line.setPosition(m_vertices[i]);
        line.setRotation(segment.angle());
        line.setFillColor(m_color);
        target.draw(line);
    }

    // Draw circles at vertices to smooth sharp corners
    for (const auto& vertex : m_vertices) {
        auto circle = sf::CircleShape(stroke / 2.0f, 8);
        circle.setOrigin({ circle.getRadius(), circle.getRadius() });
        circle.setPosition(vertex);
        circle.setFillColor(m_color);
        target.draw(circle);
    }
}
