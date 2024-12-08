#pragma once

#include <span>
#include <vector>

struct DftData {
    float amplitude;
    float frequency;
    float phase;
};

auto discrete_fourier_transform(std::span<const float> signal) -> std::vector<DftData>;
