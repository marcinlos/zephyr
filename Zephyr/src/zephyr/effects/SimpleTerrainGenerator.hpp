/**
 * @file SimpleTerrainGenerator.hpp
 */

#ifndef ZEPHYR_EFFECTS_SIMPLETERRAINGENERATOR_HPP_
#define ZEPHYR_EFFECTS_SIMPLETERRAINGENERATOR_HPP_

#include <zephyr/effects/TerrainGenerator.hpp>

#include <cstdlib>
#include <queue>

namespace zephyr {
namespace effects {

class SimpleTerrainGenerator : public TerrainGenerator {
public:

    SimpleTerrainGenerator(float extent, int iterations, float h)
    : TerrainGenerator(extent, 1 << iterations)
    , iterations(iterations)
    , h(h)
    { }

private:

    struct Job {
        Grid grid;
        int level;
        float scale;

        Job(Grid grid, int level, float scale)
        : grid(grid), level(level), scale(scale)
        { }
    };

    std::queue<Job> jobs;

    void modify() override {
        std::srand(std::time(nullptr));
        jobs.emplace(v, iterations, h);
        while (! jobs.empty()) {
            Job job = jobs.front();
            jobs.pop();
            step(job.grid, job.level, job.scale);
        }
        for (int i = 0; i < 5; ++ i) {
            smooth(0.8f);
        }
        for (int i = 0; i < 3; ++ i) {
            smoothColors(0.6f);
        }

        colorFromHeight();
    }

    void step(Grid grid, int level, float scale) {
        int n = 1 << level;

        auto a = grid[0][0];
        auto b = grid[0][n];
        auto c = grid[n][0];
        auto d = grid[n][n];

        int k = n / 2;

        auto center = grid[k][k];
        center.y = (a.y + b.y + c.y + d.y) / 4 + random() * scale;

        int bi = grid.offset / grid.inRow;
        int bj = grid.offset % grid.inRow;

        square(grid, 0, k, k, scale);
        square(grid, k, 0, k, scale);
        square(grid, k, n, k, scale);
        square(grid, n, k, k, scale);

        if (level > 1) {
            jobs.emplace(grid.startAt(0, 0), level - 1, scale / 2);
            jobs.emplace(grid.startAt(0, k), level - 1, scale / 2);
            jobs.emplace(grid.startAt(k, 0), level - 1, scale / 2);
            jobs.emplace(grid.startAt(k, k), level - 1, scale / 2);
        }

    }

    void square(Grid grid, int i, int j, int n, float scale) {
        int count = 0;
        float sum = 0.0f;
        if (grid.insideFull(i - n, j)) {
            sum += grid[i - n][j].y;
            ++ count;
        }
        if (grid.insideFull(i + n, j)) {
            sum += grid[i + n][j].y;
            ++ count;
        }
        if (grid.insideFull(i, j - n)) {
            sum += grid[i][j - n].y;
            ++ count;
        }
        if (grid.insideFull(i, j + n)) {
            sum += grid[i][j + n].y;
            ++ count;
        }
        grid[i][j].y = sum / count + random() * scale;
    }

    void smooth(float k) {
        for (int i = 0; i < onEdge; ++ i) {
            for (int j = 1; j < onEdge; ++ j) {
                v[i][j].y = v[i][j - 1].y * (1 - k) + v[i][j].y * k;
            }
        }
        for (int i = 0; i < onEdge; ++ i) {
            for (int j = 1; j < onEdge; ++ j) {
                v[j][i].y = v[j - 1][i].y * (1 - k) + v[i][j].y * k;
            }
        }
        for (int i = 0; i < onEdge; ++ i) {
            for (int j = 0; j < onEdge - 1; ++ j) {
                v[i][j].y = v[i][j + 1].y * (1 - k) + v[i][j].y * k;
            }
        }
        for (int i = 0; i < onEdge; ++ i) {
            for (int j = 0; j < onEdge - 1; ++ j) {
                v[j][i].y = v[j + 1][i].y * (1 - k) + v[i][j].y * k;
            }
        }
    }


    void smoothColors(float k) {
        for (int i = 0; i < onEdge; ++ i) {
            for (int j = 1; j < onEdge; ++ j) {
                colors[i][j].y = colors[i][j - 1].y * (1 - k) + colors[i][j].y * k;
            }
        }
        for (int i = 0; i < onEdge; ++ i) {
            for (int j = 1; j < onEdge; ++ j) {
                colors[j][i].y = colors[j - 1][i].y * (1 - k) + colors[i][j].y * k;
            }
        }
        for (int i = 0; i < onEdge; ++ i) {
            for (int j = 0; j < onEdge - 1; ++ j) {
                colors[i][j].y = colors[i][j + 1].y * (1 - k) + colors[i][j].y * k;
            }
        }
        for (int i = 0; i < onEdge; ++ i) {
            for (int j = 0; j < onEdge - 1; ++ j) {
                colors[j][i].y = colors[j + 1][i].y * (1 - k) + colors[i][j].y * k;
            }
        }
    }

    void makeFlat() {
        for (int i = 0; i < onEdge; ++ i) {
            for (int j = 0; j < onEdge; ++ j) {
                v[i][j].y = 0.0f;
            }
        }
    }

    void colorFromHeight() {
        float min = 1000.0f, max = -1000.0f;
        for (int i = 0; i < onEdge; ++ i) {
            for (int j = 0; j < onEdge; ++ j) {
                float x = v[i][j].y;
                min = std::min(min, x);
                max = std::max(max, x);
            }
        }
        for (int i = 0; i < onEdge; ++ i) {
            for (int j = 0; j < onEdge; ++ j) {
                float x = v[i][j].y;
                float color = (x - min) / (max - min);
                colors[i][j].x *= color;
                colors[i][j].y *= color;
                colors[i][j].z *= color;
            }
        }
    }

    float random() const {
        return 2 * (rand() / (0.0f + RAND_MAX) - 0.5f);
    }

    int iterations;

    float h;
};


} /* namespace effects */
} /* namespace zephyr */


#endif /* ZEPHYR_EFFECTS_SIMPLETERRAINGENERATOR_HPP_ */
