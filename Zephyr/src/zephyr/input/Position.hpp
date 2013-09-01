/**
 * @file Position.hpp
 */

#ifndef ZEPHYR_INPUT_POSITION_HPP_
#define ZEPHYR_INPUT_POSITION_HPP_

#include <iostream>

namespace zephyr {
namespace input {

/**
 * Structure representing 2d point, used to denote cursor position.
 */
struct Position {
    double x;
    double y;
};


std::ostream& operator << (std::ostream& os, const Position pos);


} /* namespace input */
} /* namespace zephyr */


#endif /* ZEPHYR_INPUT_POSITION_HPP_ */
