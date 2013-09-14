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

    Grid(Iter begin, Iter end, std::size_t row)
    : begin_(begin)
    , end_(end)
    , count_(std::distance(begin, end))
    , rowSize_(row)
    { }

    class ConstRowProxy {
    public:

        ConstRowProxy(Grid& grid, int num)
        : grid_(grid)
        , num_(num)
        { }

        const value_type& operator [] (int n) const {
            return grid_.get(index(n));
        }

    protected:

        int index(int n) const {
            return grid_.rowSize_ * num_ + n;
        }

        int num_;
        Grid& grid_;
    };

    class RowProxy: private ConstRowProxy {
    public:

        RowProxy(Grid& grid, int num)
        : ConstRowProxy { grid, num }
        { }

        value_type& operator [] (int n) {
            return ConstRowProxy::grid_.get(ConstRowProxy::index(n));
        }
    };

    bool contains(int i, int j) {
        return i * rowSize_ + j < count_;
    }

    RowProxy operator [] (int i) {
        return RowProxy { *this, i };
    }

    ConstRowProxy operator [] (int i) const {
        return ConstRowProxy { *this, i };
    }

    Grid subgrid(int i, int j) {
        return Grid { begin_ + i * rowSize_ + j, end_, rowSize_ };
    }

    std::size_t rowSize() const {
        return rowSize_;
    }

private:

    value_type& get(int i) {
        return *(begin_ + i);
    }

    const value_type& get(int i) const {
        return *(begin_ + i);
    }


    Iter begin_;
    Iter end_;
    std::size_t count_;
    std::size_t rowSize_;
};


} /* namespace effects */
} /* namespace zephyr */

#endif /* ZEPHYR_EFFECTS_GRID_HPP_ */
