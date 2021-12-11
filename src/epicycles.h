#pragma once

#include "dft.h"
#include "epicycle.h"

class Epicycles : public sf::Drawable {
    std::vector<Epicycle> m_epicycles;

public:
    Epicycles() = default;
    Epicycles(const std::vector<DftData>& dft_data, const sf::Vector2f& position, const float phase);
    void Update();
    void draw(sf::RenderTarget& target, sf::RenderStates) const override;

    [[nodiscard]] auto Empty() const { return m_epicycles.empty(); }
    [[nodiscard]] auto Size() const { return m_epicycles.size(); }
    [[nodiscard]] auto Tip() const { return m_epicycles.back().GetPosition(); }
};
