#include "dft.h"

#include <complex>

static constexpr auto pi = 3.1415926f;

auto DiscreteFourierTransform(const std::vector<float>& values) -> std::vector<DftData>
{
    const auto size = (float)values.size();
    auto transformed = std::vector<DftData>();
    for (size_t k = 0; k < values.size(); ++k) {
        auto val = std::complex<float>();
        for (size_t n = 0; n < values.size(); ++n) {
            const auto phi = (2.0f * pi * (float)k * (float)n) / size;
            val += values[n] * std::complex(std::cos(phi), -std::sin(phi));
        }
        val /= size;
        transformed.push_back({ std::abs(val), (float)k, std::arg(val) });
    }
    return transformed;
}
