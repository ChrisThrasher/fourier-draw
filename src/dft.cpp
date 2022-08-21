#include "dft.h"

#include <complex>

static constexpr auto pi = 3.1415926f;

auto discrete_fourier_transform(const std::vector<float>& signal) -> std::vector<DftData>
{
    const auto size = float(signal.size());
    auto transformed = std::vector<DftData>(signal.size());
    for (size_t k = 0; k < signal.size(); ++k) {
        auto val = std::complex<float>();
        for (size_t n = 0; n < signal.size(); ++n) {
            const auto phi = (2.0f * pi * float(k) * float(n)) / size;
            val += signal[n] * std::complex(std::cos(phi), -std::sin(phi));
        }
        val /= size;
        transformed[k] = { std::abs(val), float(k), std::arg(val) };
    }
    return transformed;
}
