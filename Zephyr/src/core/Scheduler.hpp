/*
 * Scheduler.hpp
 *
 *  Created on: Jun 27, 2013
 *      Author: los
 */

#ifndef ZEPHYR_CORE_SCHEDULER_HPP_
#define ZEPHYR_CORE_SCHEDULER_HPP_

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

/**
 * Generic task scheduler, the very bottom of the application.
 */
class Scheduler
{
public:
    /** Type of the task identifier */
    typedef std::string task_id;
    /** Type of the waiting condition identifier */
    typedef std::string queue_id;
    /** Type of the task pointer */
    typedef std::shared_ptr<Task> task_ptr;

    /** Creates a new scheduler */
    Scheduler();

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
        task_ptr task;
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

    /** Type of the heterogenous operation container */
    typedef boost::variant<
        start_task_cmd,
        stop_task_cmd,
        suspend_task_cmd,
        notify_cmd
    > operation;

    /** Functions performing actual work */
    /// @{
    void do_start_task_(const task_id& name, int priority, const task_ptr& task);
    void do_stop_task_(const task_id& name);
    void do_suspend_task_(const task_id& name, const queue_id& condition);
    void do_notify_(const queue_id& condition);
    /// @}

    /** Helper class, executing the delayed operations */
    struct Executor: public boost::static_visitor<void>
    {
        Scheduler* const scheduler;

        Executor(Scheduler* scheduler)
        : scheduler(scheduler)
        { }

        void operator ()(const start_task_cmd& cmd);
        void operator ()(const stop_task_cmd& cmd);
        void operator ()(const suspend_task_cmd& cmd);
        void operator ()(const notify_cmd& cmd);
    } executor;

    /** Loops through active tasks and calls `update()` */
    void update_all_();

    /** Schedules the operation */
    void post_operation_(operation&& op);

    /** Executes operations stored in the queue */
    void execute_delayed_operations_();

    /** Performs a single delayed operation */
    void perform_operation_(const operation& op);

    /** Container used to store active tasks */
    typedef std::list<task_info> task_list;

    /** Container used to store suspended tasks */
    typedef std::queue<task_info> task_queue;

    /** Inserts the task in the appropriate place in the task list */
    void insert_into_active_list_(task_info&& task);
    
    /** Searches for task with supplied id and removes it */
    task_list::iterator search_in_active_list_(const task_id& id);

    /**
     * Searches for condition queue with supplied id, creates the queue and
     * inserts it into the map if it does not exist.
     */
    task_queue& get_condition_queue_(const queue_id& id);

    /** Ordered list of active tasks */
    task_list active_;

    /** List of wait conditions and associated suspended tasks */
    std::unordered_map<queue_id, task_queue> suspended_;

    /**
     * Mutex protecting pending operations list. It's recursive to allow
     * operations inside task callbacks for scheduling event.
     */
    std::recursive_mutex operations_mutex_;

    /** Flag indicating whether the execution shall continue */
    std::atomic_bool is_running_;

    /** Queue of delayed operations */
    std::queue<operation> operation_queue_;
};

} /* namespace core */
} /* namespace zephyr */

#endif /* ZEPHYR_CORE_SCHEDULER_HPP_ */
