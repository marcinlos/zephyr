/**
 * @file Grid.hpp
 */

#ifndef ZEPHYR_EFFECTS_GRID_HPP_
#define ZEPHYR_EFFECTS_GRID_HPP_

#include <iterator>


namespace zephyr {
namespace effects {

template <typename Iter>
class Grid {
public:

    typedef typename std::iterator_traits<Iter>::value_type value_type;
    typedef Iter iter_type;

    Grid(Iter begin, std::size_t cols, std::size_t rows, std::ptrdiff_t offset = 0)
    : begin_(begin)
    , cols_(cols)
    , rows_(rows)
    , offset_(offset)
    { }


    class ConstRowProxy {
    public:

        ConstRowProxy(Iter begin)
        : begin_(begin)
        { }

        const value_type& operator [] (int n) const {
            return *(begin_ + n);
        }

    protected:

        Iter begin_;
    };

    class RowProxy: private ConstRowProxy {
    public:

        RowProxy(Iter begin)
        : ConstRowProxy { begin }
        { }

        value_type& operator [] (int n) {
            return *(ConstRowProxy::begin_ + n);
        }
    };

    ConstRowProxy operator [] (int row) const {
        return ConstRowProxy { begin_ + offset(row, 0) };
    }

    RowProxy operator [] (int row) {
        return RowProxy { begin_ + offset(row, 0) };
    }

    Grid subgrid(int i, int j) const {
        return Grid { begin_, cols_, rows_, offset(i, j) };
    }

    bool fullContains(int i, int j) const {
        int firstRow = offset_ / cols_;
        int firstCol = offset_ % cols_;
        i += firstRow;
        j += firstCol;
        if (i< 0 || i >= cols_) {
            return false;
        } else if (j < 0|| j >= rows_) {
            return false;
        } else {
            return true;
        }
    }

    std::size_t cols() const {
        return cols_;
    }

    std::size_t rows() const {
        return rows_;
    }


private:
    std::ptrdiff_t offset(int i, int j) const {
        return offset_ + i * cols_ + j;
    }

    Iter begin_;
    std::size_t cols_;
    std::size_t rows_;
    std::ptrdiff_t offset_;
};


template <typename Iter>
Grid<Iter> make_grid(Iter begin, Iter end, std::size_t cols) {
    std::size_t total = std::distance(begin, end);
    return Grid<Iter> { begin, cols, total / cols };
}

template <typename Cont>
auto make_grid(Cont& cont, std::size_t row) -> Grid<decltype(begin(cont))> {
    return make_grid(begin(cont), end(cont), row);
}


} /* namespace effects */
} /* namespace zephyr */

#endif /* ZEPHYR_EFFECTS_GRID_HPP_ */
