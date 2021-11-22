#pragma once

#include <vector>

struct DftData {
    float amplitude;
    float frequency;
    float phase;
};

auto DiscreteFourierTransform(const std::vector<float>& values) -> std::vector<DftData>;
