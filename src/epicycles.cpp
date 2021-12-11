#include "epicycles.h"

#include <algorithm>

Epicycles::Epicycles(const std::vector<DftData>& dft_data, const sf::Vector2f& position, const float phase)
{
    for (const auto& dft_datum : dft_data)
        m_epicycles.emplace_back(dft_datum.amplitude, dft_datum.frequency, dft_datum.phase + phase);
    std::stable_sort(m_epicycles.begin(), m_epicycles.end());
    m_epicycles.front().set_position(position);
}

void Epicycles::update()
{
    const auto dt = 2.0f * pi / (float)m_epicycles.size();
    for (size_t i = 0; i < m_epicycles.size(); ++i) {
        if (i == 0)
            m_epicycles[i].update(dt);
        else
            m_epicycles[i].update(dt, m_epicycles[i - 1].get_position());
    }
}

void Epicycles::draw(sf::RenderTarget& target, sf::RenderStates) const
{
    for (const auto& epicycle : m_epicycles)
        target.draw(epicycle);
}
