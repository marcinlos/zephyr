/**
 * @file ActionScheduler.hpp
 */

#ifndef ZEPHYR_TIME_ACTIONSCHEDULER_HPP_
#define ZEPHYR_TIME_ACTIONSCHEDULER_HPP_

#include <functional>


namespace zephyr {
namespace time {

typedef std::function<void ()> Action;

class ActionScheduler {
public:

    ActionScheduler(double initTime = 0)
    : lastUpdate(initTime)
    { }

    void scheduleIn(Action action, double fromNow) {
        queue.push({lastUpdate + fromNow, action});
    }

    void update(double t) {
        lastUpdate = t;

        while (nextShouldFire(t)) {
            const ActionEntry& entry = queue.top();
            entry.action();
            queue.pop();
        }
    }

private:

    bool nextShouldFire(double t) {
        return !queue.empty() && queue.top().scheduledAt < t;
    }

    struct ActionEntry {
        double scheduledAt;
        std::function<void ()> action;

        friend bool operator < (const ActionEntry& a, const ActionEntry& b) {
            return a.scheduledAt > b.scheduledAt;
        }
    };

    std::priority_queue<ActionEntry> queue;

    double lastUpdate = 0.0;
};

template <typename Fun>
struct Rescheduler {
    ActionScheduler& scheduler;
    Fun fun;
    double tick;

    Rescheduler(ActionScheduler& scheduler, Fun fun, double tick)
    : scheduler(scheduler)
    , fun(std::move(fun))
    , tick(tick)
    { }

    void operator () () const {
        fun();
        scheduler.scheduleIn(*this, tick);
    }
};

template <typename Fun>
Rescheduler<Fun> repeatedly(ActionScheduler& scheduler, Fun&& fun, double tick) {
    return Rescheduler<Fun>(scheduler, std::forward<Fun>(fun), tick);
}



} /* namespace time */
} /* namespace zephyr */


#endif /* ZEPHYR_TIME_ACTIONSCHEDULER_HPP_ */
