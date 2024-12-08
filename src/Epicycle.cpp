#include "Epicycle.hpp"

#include <cmath>

Epicycle::Epicycle(const float amplitude, const float frequency, const sf::Angle phase)
    : m_amplitude(amplitude)
    , m_frequency(frequency)
    , m_phase(phase)
{
}

void Epicycle::update(const float dt) { update(dt, m_position); }

void Epicycle::update(const float dt, const sf::Vector2f position)
{
    m_position = position;
    m_phase = sf::radians(m_frequency * dt + m_phase.asRadians()).wrapUnsigned();
}

void Epicycle::draw(sf::RenderTarget& target, sf::RenderStates /* states */) const
{
    auto circle = sf::CircleShape(m_amplitude, 60);
    circle.setOrigin({ circle.getRadius(), circle.getRadius() });
    circle.setPosition(m_position);
    circle.setFillColor(sf::Color::Transparent);
    circle.setOutlineColor(color);
    circle.setOutlineThickness(stroke);
    target.draw(circle);

    auto line = sf::RectangleShape({ circle.getRadius(), stroke });
    line.setOrigin({ 0, line.getSize().y / 2 });
    line.setPosition(m_position);
    line.setRotation(m_phase);
    line.setFillColor(color);
    target.draw(line);
}

void Epicycle::set_position(const sf::Vector2f position) { m_position = position; }

auto Epicycle::get_position() const -> sf::Vector2f { return m_position + sf::Vector2f(m_amplitude, m_phase); }

auto operator<(const Epicycle& lhs, const Epicycle& rhs) -> bool { return lhs.m_amplitude > rhs.m_amplitude; }
