/*
 * Speaker.hpp
 *
 *  Created on: Jun 28, 2013
 *      Author: los
 */

#ifndef SPEAKER_HPP_
#define SPEAKER_HPP_

#include <zephyr/core/Task.hpp>
#include <string>
#include <iostream>

class Speaker: public zephyr::core::Task
{
private:
    std::string text;
    int counter;

    static const int DELAY = 1000000000;

public:
    Speaker(const std::string& text)
    : text(text)
    , counter(0)
    { }

    // Override
    virtual void start() override
    {
        std::cout << "[Speaker] " << text << ": start" << std::endl;
    }

    // Override
    virtual void stop() override
    {
        std::cout << "[Speaker] " << text << ": stop" << std::endl;
    }

    // Override
    virtual void suspend() override
    {
        std::cout << "[Speaker] " << text << ": suspend" << std::endl;
    }

    // Override
    virtual void resume() override
    {
        std::cout << "[Speaker] " << text << ": resmue" << std::endl;
    }

    // Override
    virtual void update() override
    {
        if ((counter = (counter + 1) % DELAY) == 0)
        {
            std::cout << "[Speaker] " << text << ": tick" << std::endl;
        }
    }
};

#endif /* SPEAKER_HPP_ */
