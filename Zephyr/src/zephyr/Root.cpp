/*
 * Root.cpp
 *
 *  Created on: Jun 29, 2013
 *      Author: los
 */

#include <zephyr/Root.hpp>

namespace zephyr
{

Root::Root()
{
    // TODO Auto-generated constructor stub
}


void Root::run()
{
    scheduler_.run();
}

} /* namespace zephyr */
