/**
 * @file Position.cpp
 *
 * Usage of boost::io isolated to reduce compile time overhead.
 */

#include <zephyr/input/Position.hpp>
#include <boost/io/ios_state.hpp>


namespace zephyr {
namespace input {


std::ostream& operator << (std::ostream& os, const Position pos) {
    boost::io::ios_all_saver guard(os);
    os.precision(1);
    std::fixed(os);
    return os << "(" << pos.x << ", " << pos.y << ")";
}


} /* namespace input */
} /* namespace zephyr */




