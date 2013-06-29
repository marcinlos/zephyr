/*
 * Scheduler.cpp
 *
 *  Created on: Jun 27, 2013
 *      Author: los
 */

#include <zephyr/core/Scheduler.hpp>
#include <thread>
#include <stdexcept>
#include <algorithm>


namespace zephyr
{
namespace core
{

Scheduler::Scheduler()
: executor(this)
, is_running_(false)
{ }


void Scheduler::startTask(const task_id& name, int priority,
        const task_ptr& task)
{
    post_operation_(start_task_cmd {name, priority, task});
}

void Scheduler::stopTask(const task_id& name)
{
    post_operation_(stop_task_cmd {name});
}

void Scheduler::suspendTask(const task_id& name, const queue_id& condition)
{
    post_operation_(suspend_task_cmd {name, condition});
}

void Scheduler::notify(const queue_id& condition)
{
    post_operation_(notify_cmd {condition});
}

void Scheduler::run()
{
    // set the flag
    is_running_ = true;

    // main loop
    while (is_running_)
    {
        update_all_();
        execute_delayed_operations_();
    }
}

void Scheduler::stop()
{
    // breaks the main loop at the next iteration
    is_running_ = false;
}

void Scheduler::do_start_task_(const task_id& name, int priority,
        const task_ptr& task)
{
    try
    {
        task->start();
        insert_into_active_list_({name, priority, task});
    }
    catch (const std::exception& e)
    {
        std::cerr << "[Scheduler] In " << name << " start(): ";
        std::cerr << "exception: " << e.what() << std::endl;
    }
    catch (...)
    {
        std::cerr << "[Scheduler] In " << name << " start(): ";
        std::cerr << "Unknown exception!" << std::endl;
    }
}

void Scheduler::do_stop_task_(const task_id& name)
{
    auto i = search_in_active_list_(name);
    if (i != active_.end())
    {
        try
        {
            i->task->stop();
            active_.erase(i);
        }
        catch (const std::exception& e)
        {
            std::cerr << "[Scheduler] In " << name << " stop(): ";
            std::cerr << "exception: " << e.what() << std::endl;
        }
        catch (...)
        {
            std::cerr << "[Scheduler] In " << name << " stop(): ";
            std::cerr << "Unknown exception!" << std::endl;
        }
    }
    else
    {
        std::cerr << "[Scheduler] Warning: attempt to stop nonexistant task: "
                << name << std::endl;
    }
}

void Scheduler::do_suspend_task_(const task_id& name, const queue_id& condition)
{
    auto i = search_in_active_list_(name);
    if (i != active_.end())
    {
        try
        {
            i->task->suspend();
            // actually suspend only if the callback was successful
            task_queue& queue = get_condition_queue_(condition);
            queue.push(std::move(*i));
            active_.erase(i);
        }
        catch (const std::exception& e)
        {
            std::cerr << "[Scheduler] In " << name << " suspend(): ";
            std::cerr << "exception: " << e.what() << std::endl;
        }
        catch (...)
        {
            std::cerr << "[Scheduler] In " << name << " suspend(): ";
            std::cerr << "Unknown exception!" << std::endl;
        }
    }
    else
    {
        std::cerr << "[Scheduler] Warning: attempt to suspend nonexistant "
                << "task: " << name << std::endl;
    }

}

void Scheduler::do_notify_(const queue_id& condition)
{
    auto cond_iter = suspended_.find(condition);
    if (cond_iter != suspended_.end())
    {
        // remove the queue from the map, this way potential suspends on the
        // same condition will be in a different queue
        task_queue queue(std::move(cond_iter->second));
        suspended_.erase(cond_iter);

        while (! queue.empty())
        {
            task_info& info = queue.front();
            try
            {
                info.task->resume();
                insert_into_active_list_(std::move(info));
            }
            catch (const std::exception& e)
            {
                std::cerr << "[Scheduler] In " << info.name << " resume(): ";
                std::cerr << "exception: " << e.what() << std::endl;
            }
            catch (...)
            {
                std::cerr << "[Scheduler] In " << info.name << " resume(): ";
                std::cerr << "Unknown exception!" << std::endl;
            }
            // remove from the wait queue regardless of resume success
            queue.pop();
        }
    }
}

void Scheduler::Executor::operator ()(const start_task_cmd& cmd)
{
    scheduler->do_start_task_(cmd.name, cmd.priority, cmd.task);
}

void Scheduler::Executor::operator ()(const stop_task_cmd& cmd)
{
    scheduler->do_stop_task_(cmd.name);
}

void Scheduler::Executor::operator ()(const suspend_task_cmd& cmd)
{
    scheduler->do_suspend_task_(cmd.name, cmd.condition);
}

void Scheduler::Executor::operator ()(const notify_cmd& cmd)
{
    scheduler->do_notify_(cmd.condition);
}

void Scheduler::update_all_()
{
    for (const task_info& task : active_)
    {
        try
        {
            task.task->update();
        }
        catch (const std::exception& e)
        {
            std::cerr << "[Scheduler] In " << task.name << ": ";
            std::cerr << "exception: " << e.what() << std::endl;
        }
        catch (...)
        {
            std::cerr << "[Scheduler] In " << task.name << ": ";
            std::cerr << "Unknown exception!" << std::endl;
        }
    }
}

void Scheduler::post_operation_(operation&& op)
{
    std::lock_guard<std::recursive_mutex> lock(operations_mutex_);
    operation_queue_.push(std::move(op));
}

void Scheduler::execute_delayed_operations_()
{
    std::lock_guard<std::recursive_mutex> lock(operations_mutex_);

    // drain the queue, it may grow due to operations
    while (!operation_queue_.empty())
    {
        const operation& op = operation_queue_.front();
        perform_operation_(op);
        operation_queue_.pop();
    }
}

void Scheduler::perform_operation_(const operation& op)
{
    // dispatch to visitor
    boost::apply_visitor(executor, op);
}

void Scheduler::insert_into_active_list_(task_info&& task)
{
    auto i = active_.begin();
    while (i != active_.end() && i->priority > task.priority)
    {
        ++i;
    }
    active_.insert(i, std::move(task));
}

auto Scheduler::search_in_active_list_(const task_id& id) -> task_list::iterator
{
    return std::find_if(active_.begin(), active_.end(),
        [id](const task_info& info) { return info.name == id; });
}

auto Scheduler::get_condition_queue_(const queue_id& id) -> task_queue&
{
    auto i = suspended_.find(id);
    if (i == suspended_.end())
    {
        // create & insert
        i = suspended_.insert(std::make_pair(id, task_queue())).first;
    }
    return i->second;
}

} /* namespace core */
} /* namespace zephyr */
