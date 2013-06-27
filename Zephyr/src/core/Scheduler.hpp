/*
 * Scheduler.hpp
 *
 *  Created on: Jun 27, 2013
 *      Author: los
 */

#ifndef SCHEDULER_HPP_
#define SCHEDULER_HPP_

#include "Task.hpp"
#include <string>
#include <memory>
#include <mutex>
#include <atomic>
#include <list>
#include <queue>
#include <unordered_map>
#include <boost/variant.hpp>

namespace zephyr
{
namespace core
{

class Scheduler
{
public:

    /** Type of the task identifier */
    typedef std::string task_id;
    /** Type of the waiting condition identifier */
    typedef std::string queue_id;
    /** Type of the task pointer */
    typedef std::shared_ptr<Task> task_ptr;

    Scheduler();

    ~Scheduler();

    void startTask(const task_id& name, int priority, const task_ptr& task);

    void stopTask(const task_id& name);

    void suspendTask(const task_id& name, const queue_id& condition);

    void notify(const queue_id& condition);

    void run();

    void stop();

private:
    /** Task data */
    struct task_info
    {
        task_id name;
        int priority;
        task_ptr& task;
    };

    /**
     * Structures for keeping delayed operations' arguments
     */
    /// @{
    struct start_task_cmd
    {
        task_id name;
        int priority;
        task_ptr task;
    };

    struct stop_task_cmd
    {
        task_id name;
    };

    struct suspend_task_cmd
    {
        task_id name;
        queue_id condition;
    };

    struct notify_cmd
    {
        queue_id condition;
    };
    /// @}

    /** Ordered list of active tasks */
    std::list<task_info> active;
    /** List of suspended tasks */
    std::unordered_map<task_id, task_info> suspended;

    /** Mutex protecting main loop execution */
    std::mutex loop_mutex;
    /**
     * Mutex protecting pending operations list. It's recursive to allow
     * operations inside task callbacks for scheduling event.
     */
    std::recursive_mutex operations_mutex;

    /** Flag indicating whether the execution shall continue */
    std::atomic_bool is_running;

    typedef boost::variant<
        start_task_cmd,
        stop_task_cmd,
        suspend_task_cmd,
        notify_cmd
    > operation;

    /** Queue of delayed operations */
    std::queue<operation> operation_queue;
};

} /* namespace core */
} /* namespace zephyr */
#endif /* SCHEDULER_HPP_ */
