/**
 * @file TaskletScheduler.hpp
 */

#ifndef ZEPHYR_TIME_TASKLETSCHEDULER_HPP_
#define ZEPHYR_TIME_TASKLETSCHEDULER_HPP_

#include <functional>
#include <list>

namespace zephyr {
namespace time {


class TaskletScheduler {
public:
    typedef std::function<bool (double, double)> Tasklet;
    typedef std::list<Tasklet> Cont;
    typedef Cont::iterator TaskletId;


    TaskletId add(Tasklet tasklet) {
        tasklets.push_front(std::move(tasklet));
        return tasklets.begin();
    }

    void remove(TaskletId id) {
        tasklets.erase(id);
    }

    void update(double time, double dt) {
        for (TaskletId it = begin(tasklets); it != end(tasklets); ) {
            TaskletId current = it++;
            if (! (*current)(time, dt)) {
                remove(current);
            }
        }
    }

private:
    std::list<Tasklet> tasklets;
};


} /* namespace time */
} /* namespace zephyr */

#endif /* ZEPHYR_TIME_TASKLETSCHEDULER_HPP_ */
