#pragma once

#include <SFML/Graphics.hpp>

static constexpr auto pi = 3.1415926f;
static constexpr auto half_pi = pi / 2.0f;
static constexpr auto stroke = 1.f;
static const sf::Color color(255, 255, 255, 64);

class Epicycle : public sf::Drawable {
    sf::Vector2f m_position;
    float m_amplitude;
    float m_frequency;
    float m_phase;

public:
    Epicycle(const float amplitude, const float frequency, const float phase);

    void update(const float dt);
    void update(const float dt, const sf::Vector2f& position);
    void draw(sf::RenderTarget& target, sf::RenderStates) const override;

    void set_position(const sf::Vector2f& position);
    [[nodiscard]] auto get_position() const -> sf::Vector2f;

    friend auto operator<(const Epicycle& lhs, const Epicycle& rhs) -> bool;
};
