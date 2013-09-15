/**
 * @file NoiseSmoother.hpp
 */

#ifndef ZEPHYR_EFFECTS_NOISESMOOTHER_HPP_
#define ZEPHYR_EFFECTS_NOISESMOOTHER_HPP_


namespace zephyr {
namespace effects {

namespace detail {

    template <typename Array>
    void smoothStep(Array& v, std::size_t n, float k) {
        for (int i = 0; i < n; ++ i) {
            for (int j = 1; j < n; ++ j) {
                v[i][j] = v[i][j - 1] * (1 - k) + v[i][j] * k;
            }
        }
        for (int i = 0; i < n; ++ i) {
            for (int j = 1; j < n; ++ j) {
                v[j][i] = v[j - 1][i] * (1 - k) + v[j][i] * k;
            }
        }
        for (int i = 0; i < n; ++ i) {
            for (int j = 0; j < n - 1; ++ j) {
                v[i][j] = v[i][j + 1] * (1 - k) + v[i][j] * k;
            }
        }
        for (int i = 0; i < n; ++ i) {
            for (int j = 0; j < n - 1; ++ j) {
                v[j][i] = v[j + 1][i] * (1 - k) + v[j][i] * k;
            }
        }
    }
} /* namespace detail */

template <typename Array>
void simpleSmooth(Array& v, std::size_t row, float k, int iters = 1) {
    for (int i = 0; i < iters; ++ i) {
        detail::smoothStep(v, row, k);
    }
}


} /* namespace effects */
} /* namespace zephyr */

#endif /* ZEPHYR_EFFECTS_NOISESMOOTHER_HPP_ */
