#pragma once

#include <SFML/Graphics.hpp>

static constexpr auto stroke = 1.f;
static constexpr sf::Color color(255, 255, 255, 64);

class Epicycle : public sf::Drawable {
    sf::Vector2f m_position;
    float m_amplitude;
    float m_frequency;
    sf::Angle m_phase;

public:
    Epicycle(float amplitude, float frequency, const sf::Angle& phase);

    void update(float dt);
    void update(float dt, const sf::Vector2f& position);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void set_position(const sf::Vector2f& position);
    [[nodiscard]] auto get_position() const -> sf::Vector2f;

    friend auto operator<(const Epicycle& lhs, const Epicycle& rhs) -> bool;
};
