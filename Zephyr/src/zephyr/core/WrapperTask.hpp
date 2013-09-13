/**
 * @file WrapperTask.hpp
 */

#ifndef ZEPHYR_CORE_WRAPPERTASK_HPP_
#define ZEPHYR_CORE_WRAPPERTASK_HPP_

#include <zephyr/core/Task.hpp>

namespace zephyr {
namespace core {


template <typename Fun>
class WrapperTask : public Task {
public:
    WrapperTask(Fun fun)
    : fun_ { std::move(fun) }
    { }

    void update() override {
        fun_();
    }

private:
    Fun fun_;
};


template <typename Fun>
TaskPtr wrapAsTask(Fun&& fun) {
    return std::make_shared<WrapperTask<Fun>>(std::forward<Fun>(fun));
}


} /* namespace core */
} /* namespace zephyr */


#endif /* ZEPHYR_CORE_WRAPPERTASK_HPP_ */
