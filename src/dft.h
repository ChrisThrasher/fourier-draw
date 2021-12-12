#pragma once

#include <vector>

struct DftData {
    float amplitude;
    float frequency;
    float phase;
};

auto discrete_fourier_transform(const std::vector<float>& signal) -> std::vector<DftData>;
