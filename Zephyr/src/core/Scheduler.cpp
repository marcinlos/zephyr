/*
 * Scheduler.cpp
 *
 *  Created on: Jun 27, 2013
 *      Author: los
 */

#include "Scheduler.hpp"


namespace zephyr
{
namespace core
{

Scheduler::Scheduler()
: is_running(false)
{
    // TODO Auto-generated constructor stub

}

Scheduler::~Scheduler()
{
    // TODO Auto-generated destructor stub
}

void Scheduler::startTask(const task_id& name, int priority,
        const task_ptr& task)
{

}

void Scheduler::stopTask(const task_id& name)
{

}

void Scheduler::suspendTask(const task_id& name, const queue_id& condition)
{

}

void Scheduler::notify(const queue_id& condition)
{

}

void Scheduler::run()
{
    is_running = true;

    // main loop
    while (is_running)
    {
        std::lock_guard<std::mutex> guard(loop_mutex);
    }
}

void Scheduler::stop()
{
    // sets the flag to false
    is_running = false;
}

} /* namespace core */
} /* namespace zephyr */
