#pragma once

#include "Dft.hpp"
#include "Epicycle.hpp"

class Epicycles : public sf::Drawable {
    std::vector<Epicycle> m_epicycles;

public:
    Epicycles() = default;
    Epicycles(std::span<const DftData> dft_data, sf::Vector2f position, sf::Angle phase);
    void update();
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    [[nodiscard]] auto empty() const { return m_epicycles.empty(); }
    [[nodiscard]] auto size() const { return m_epicycles.size(); }
    [[nodiscard]] auto tip() const { return m_epicycles.back().get_position(); }
};
