#include "epicycle.h"

#include <cmath>

Epicycle::Epicycle(const float amplitude, const float frequency, const float phase)
    : m_amplitude(amplitude)
    , m_frequency(frequency)
    , m_phase(phase)
{
}

void Epicycle::update(const float dt) { update(dt, m_position); }

void Epicycle::update(const float dt, const sf::Vector2f& position)
{
    m_position = position;
    m_phase = fmodf(m_frequency * dt + m_phase, 2 * pi);
}

void Epicycle::draw(sf::RenderTarget& target, sf::RenderStates) const
{
    constexpr auto to_degrees = 180.0f / pi;
    constexpr auto stroke = 2.5f;
    const auto color = sf::Color(255, 255, 255, 128);

    auto circle = sf::CircleShape(m_amplitude, 60);
    circle.setOrigin({ circle.getRadius(), circle.getRadius() });
    circle.setPosition(m_position);
    circle.setFillColor(sf::Color::Transparent);
    circle.setOutlineColor(color);
    circle.setOutlineThickness(stroke);
    target.draw(circle);

    auto line = sf::RectangleShape({ circle.getRadius(), stroke });
    line.setOrigin({ 0.0f, line.getSize().y / 2.0f });
    line.setPosition(m_position);
    line.setRotation(m_phase * to_degrees);
    line.setFillColor(color);
    target.draw(line);
}

void Epicycle::set_position(const sf::Vector2f& position) { m_position = position; }

auto Epicycle::get_position() const -> sf::Vector2f
{
    return m_position + m_amplitude * sf::Vector2f(std::cos(m_phase), std::sin(m_phase));
}

auto operator<(const Epicycle& lhs, const Epicycle& rhs) -> bool { return lhs.m_amplitude > rhs.m_amplitude; }
