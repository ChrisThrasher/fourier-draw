#pragma once

#include "dft.h"
#include "epicycle.h"

class Epicycles : public sf::Drawable {
    std::vector<Epicycle> m_epicycles;

public:
    Epicycles() = default;
    Epicycles(const std::vector<DftData>& dft_data, const sf::Vector2f& position, const float phase);
    void update();
    void draw(sf::RenderTarget& target, sf::RenderStates) const override;

    [[nodiscard]] auto empty() const { return m_epicycles.empty(); }
    [[nodiscard]] auto size() const { return m_epicycles.size(); }
    [[nodiscard]] auto tip() const { return m_epicycles.back().get_position(); }
};
