#include "line.h"

#include <cmath>

static constexpr auto pi = 3.1415926f;

void Line::push_back(const sf::Vector2f& vertex) { m_vertices.push_back(vertex); }

void Line::set_brightness(const sf::Uint8 brightness) { m_color = { brightness, brightness, brightness }; }

void Line::draw(sf::RenderTarget& target, sf::RenderStates /* states */) const
{
    constexpr auto stroke = 5.0f;

    // Draw line segments
    for (size_t i = 0; i + 1 < m_vertices.size(); ++i) {
        const auto segment = m_vertices[i + 1] - m_vertices[i];
        const auto length = std::sqrt(segment.x * segment.x + segment.y * segment.y);
        const auto heading = std::atan2(segment.y, segment.x);

        auto line = sf::RectangleShape({ length, stroke });
        line.setOrigin({ 0.0f, stroke / 2.0f });
        line.setPosition(m_vertices[i]);
        line.setRotation(heading * 180.0f / pi);
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
