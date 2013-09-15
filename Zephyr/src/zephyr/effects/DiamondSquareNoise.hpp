/**
 * @file DiamondSquareNoise.hpp
 */

#ifndef ZEPHYR_EFFECTS_DIAMONDSQUARENOISE_HPP_
#define ZEPHYR_EFFECTS_DIAMONDSQUARENOISE_HPP_

#include <zephyr/effects/Grid.hpp>
#include <vector>
#include <queue>
#include <random>

#include <iomanip>

namespace zephyr {
namespace effects {
namespace diamond {

    struct Params {
        int iterations;
        float variance;
    };

    namespace detail {

        template <typename Iter>
        struct Generator {

            typedef typename Grid<Iter>::value_type value_type;

            std::default_random_engine engine;
            std::uniform_real_distribution<float> randGen;

            Generator()
            : engine { static_cast<unsigned>(std::time(nullptr)) }
            , randGen { -1, 1 }
            { }

            struct Step {
                Grid<Iter> grid;
                Params params;

                Step(Grid<Iter> grid, Params params)
                : grid(grid)
                , params(params)
                { }
            };


            void compute(Grid<Iter> grid, const Params& params) {
                makeStep({ grid, params });
                while (!steps_.empty()) {
                    Step s = steps_.front();
                    steps_.pop();
                    makeStep(s);
                }
            }

            std::queue<Step> steps_;

            void makeStep(Step step) {
                Grid<Iter> grid = step.grid;
                Params params = step.params;
                int n = 1 << params.iterations;
                float a = grid[0][0];
                float b = grid[0][n];
                float c = grid[n][0];
                float d = grid[n][n];

                float var = params.variance;

                int k = n / 2;
                grid[k][k] = (a + b + c + d) / 4  + nextRand() * var;

                square(grid, 0, k, k, var);
                square(grid, k, 0, k, var);
                square(grid, k, n, k, var);
                square(grid, n, k, k, var);

                Params nextParams {
                    params.iterations - 1,
                    var / 2
                };
                if (params.iterations > 1) {
                    steps_.push({ grid.subgrid(0, 0), nextParams });
                    steps_.push({ grid.subgrid(0, k), nextParams });
                    steps_.push({ grid.subgrid(k, 0), nextParams });
                    steps_.push({ grid.subgrid(k, k), nextParams });
                }
            }

            void square(Grid<Iter> grid, int i, int j, int n, float scale) {
                int count = 0;
                float sum = 0.0f;
                if (grid.fullContains(i - n, j)) {
                    sum += grid[i - n][j];
                    ++ count;
                }
                if (grid.fullContains(i + n, j)) {
                    sum += grid[i + n][j];
                    ++ count;
                }
                if (grid.fullContains(i, j - n)) {
                    sum += grid[i][j - n];
                    ++ count;
                }
                if (grid.fullContains(i, j + n)) {
                    sum += grid[i][j + n];
                    ++ count;
                }
                grid[i][j] = sum / count + nextRand() * scale;
            }

            float nextRand() {
                return randGen(engine);
            }

        };


    }

    void noise(std::vector<float>& data, const Params& params) {
        int n = 1 << params.iterations;
        std::size_t size = (n + 1) * (n + 1);
        auto grid = make_grid(data, n + 1);

        typedef decltype(grid) GridType;
        detail::Generator<GridType::iter_type> generator;
        generator.compute(grid, params);
    }

    std::vector<float> noise(const Params& params) {
        int n = 1 << params.iterations;
        std::size_t size = (n + 1) * (n + 1);
        std::vector<float> data(size, 0);
        noise(data, params);
        return data;
    }

} /* namespace noise */
} /* namespace effects */
} /* namespace zephyr */


#endif /* ZEPHYR_EFFECTS_DIAMONDSQUARENOISE_HPP_ */
