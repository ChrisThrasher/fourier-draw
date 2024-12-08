#include "Epicycles.hpp"

#include <algorithm>

Epicycles::Epicycles(const std::span<const DftData> dft_data, const sf::Vector2f position, const sf::Angle phase)
{
    m_epicycles.reserve(dft_data.size());
    for (const auto& dft_datum : dft_data)
        m_epicycles.emplace_back(dft_datum.amplitude, dft_datum.frequency, sf::radians(dft_datum.phase) + phase);
    std::stable_sort(m_epicycles.begin(), m_epicycles.end());
    m_epicycles.front().set_position(position);
}

void Epicycles::update()
{
    const auto dt = sf::degrees(360).asRadians() / float(m_epicycles.size());
    m_epicycles.front().update(dt);
    for (std::size_t i = 1; i < m_epicycles.size(); ++i)
        m_epicycles[i].update(dt, m_epicycles[i - 1].get_position());
}

void Epicycles::draw(sf::RenderTarget& target, sf::RenderStates /* states */) const
{
    for (const auto& epicycle : m_epicycles)
        target.draw(epicycle);
}
