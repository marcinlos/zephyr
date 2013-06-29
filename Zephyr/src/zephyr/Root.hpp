/*
 * Root.hpp
 *
 *  Created on: Jun 29, 2013
 *      Author: los
 */

#ifndef ZEPHYR_ROOT_HPP_
#define ZEPHYR_ROOT_HPP_

#include <zephyr/core/Scheduler.hpp>

namespace zephyr
{

class Root
{
public:
    Root();

    void run();

    core::Scheduler& scheduler() { return scheduler_; }

private:
    core::Scheduler scheduler_;

};

} /* namespace zephyr */
#endif /* ZEPHYR_ROOT_HPP_ */
