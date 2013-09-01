/**
 * @file InitContext.hpp
 */

#ifndef ZEPHYR_INITCONTEXT_HPP_
#define ZEPHYR_INITCONTEXT_HPP_

#include <zephyr/core/Config.hpp>
#include <zephyr/core/MessageQueue.hpp>
#include <zephyr/core/MessageDispatcher.hpp>
#include <zephyr/core/Scheduler.hpp>
#include <zephyr/time/ClockManager.hpp>

namespace zephyr {

/**
 * Structure containing facilities common to all the subsystems, passed to each
 * during initialization.
 */
struct Context {
    core::Config& config;
    core::MessageQueue& messageQueue;
    core::MessageDispatcher& dispatcher;
    core::Scheduler& scheduler;

    time::ClockManager& clockManager;
};

} /* namespace zephyr */

#endif /* ZEPHYR_INITCONTEXT_HPP_ */
