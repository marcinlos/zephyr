/**
 * @file Root.hpp
 */

#ifndef ZEPHYR_ROOT_HPP_
#define ZEPHYR_ROOT_HPP_

#include <zephyr/core/Scheduler.hpp>
#include <zephyr/core/Config.hpp>

namespace zephyr
{

class Root
{
public:
    /** Initializes the engine using data from the XML configuration file */
    Root(const std::string& config_stream);

    /** Initializes the engine using data from the XML configuration stream */
    Root(std::istream& config);

    /** Runs the main loop */
    void run();

    /** 
     * @return Task scheduler 
     */
    core::Scheduler& scheduler() { return scheduler_; }

    /** 
     * @return Configuration 
     */
    core::Config& config() { return config_; }

private:
    core::Scheduler scheduler_;
    core::Config config_;

};

} /* namespace zephyr */
#endif /* ZEPHYR_ROOT_HPP_ */
