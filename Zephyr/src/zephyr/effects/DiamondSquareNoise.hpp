/**
 * @file DiamondSquareNoise.hpp
 */

#ifndef ZEPHYR_EFFECTS_DIAMONDSQUARENOISE_HPP_
#define ZEPHYR_EFFECTS_DIAMONDSQUARENOISE_HPP_

#include <zephyr/effects/Grid.hpp>
#include <vector>
#include <queue>
#include <random>


namespace zephyr {
namespace effects {
namespace diamond {

    struct Params {
        int iterations;
        float variance;
    };



    std::vector<float> noise(const Params& params) {
        int n = 1 << params.iterations;
        std::vector<float> data(n * n);
        Grid<float*> grid(&data[0], &data[n * n], n);




        return data;
    }

namespace detail {

    template <typename Iter>
    struct Generator {

        typedef typename Grid<Iter>::value_type value_type;

        struct Step {
            Grid<Iter> grid;
            Params params;
        };


        void compute(Grid<Iter> grid, const Params& params) {
            step({grid, params});
            while (!steps_.empty()) {
                Step step = steps_.front();
                steps_.pop();
                step(step);
            }
        }

        std::queue<Params> steps_;

        void step(Step& step) {
            Grid<Iter>& grid = step.grid;
            const Params& params = step.params;
            int n = 1 << params.iterations;
            auto a = grid[0][0];
            auto b = grid[0][n];
            auto c = grid[n][0];
            auto d = grid[n][n];

            int k = n / 2;
            grid[k][k] = (a + b + c + d) / 4;
            int bi = grid.offset / grid.rowSize();
            int bj = grid.offset % grid.rowSize();

            float var = params.variance;
            square(grid, 0, k, k, var);
            square(grid, k, 0, k, var);
            square(grid, k, n, k, var);
            square(grid, n, k, k, var);

            Params nextParams {
                params.iterations - 1,
                params.variance / 2
            };
            if (params.iterations > 1) {
                steps_.emplace(grid.subgrid(0, 0), nextParams);
                steps_.emplace(grid.subgrid(0, k), nextParams);
                steps_.emplace(grid.subgrid(k, 0), nextParams);
                steps_.emplace(grid.subgrid(k, k), nextParams);
            }
        }

        void square(Grid<Iter> grid, int i, int j, int n, float scale) {
            int count = 0;
            float sum = 0.0f;
            if (grid.contains(i - n, j)) {
                sum += grid[i - n][j].y;
                ++ count;
            }
            if (grid.contains(i + n, j)) {
                sum += grid[i + n][j].y;
                ++ count;
            }
            if (grid.contains(i, j - n)) {
                sum += grid[i][j - n].y;
                ++ count;
            }
            if (grid.contains(i, j + n)) {
                sum += grid[i][j + n].y;
                ++ count;
            }
            grid[i][j].y = sum / count + random() * scale;
        }

    };


}



} /* namespace noise */
} /* namespace effects */
} /* namespace zephyr */


#endif /* ZEPHYR_EFFECTS_DIAMONDSQUARENOISE_HPP_ */
