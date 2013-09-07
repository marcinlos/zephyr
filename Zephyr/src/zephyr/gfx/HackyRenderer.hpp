/**
 * @file HackyRenderer.hpp
 */

#ifndef ZEPHYR_GFX_HACKYRENDERER_HPP_
#define ZEPHYR_GFX_HACKYRENDERER_HPP_

#include <zephyr/Context.hpp>
#include <zephyr/core/Task.hpp>
#include <zephyr/time/ClockManager.hpp>
#include <zephyr/time/Clock.hpp>
#include <memory>
#include <queue>

#include <zephyr/input/Key.hpp>

#include <zephyr/input/Position.hpp>


namespace zephyr {
namespace gfx {


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
        for (Tasklet& tasklet: tasklets) {
            tasklet(time, dt);
        }
        for (TaskletId it = begin(tasklets); it != end(tasklets); ) {
            TaskletId current = it++;
            if (! (*current)(time, dt)) {
                tasklets.erase(current);
            }
        }
    }

private:
    std::list<Tasklet> tasklets;
};


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



class HackyRenderer: public core::Task {
public:

    HackyRenderer(Context ctx);

    void update() override;

private:

    void initOpenGL();

    bool pressed(input::Key key) const {
        return isPressed[static_cast<int>(key)];
    }

    void inputHandler(const core::Message& msg);

    const time::ClockManager& clocks;
    const time::Clock& clock;

    TaskletScheduler taskletScheduler;
    ActionScheduler actionScheduler;

    bool vsync = true;

    input::Position cursor;


    const static int KEY_NUMBER = static_cast<int>(input::Key::LAST);

    bool isPressed[KEY_NUMBER];

    void updateTime();

    std::shared_ptr<struct SceneManager> scene;

    int counter;


};

} /* namespace gfx */
} /* namespace zephyr */

#endif /* ZEPHYR_GFX_HACKYRENDERER_HPP_ */
