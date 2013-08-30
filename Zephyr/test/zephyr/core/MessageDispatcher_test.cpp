/*
 * MessageDispatcher_test.cpp
 *
 *  Created on: Aug 29, 2013
 *      Author: los
 */

#include <zephyr/core/MessageDispatcher.hpp>
#include <functional>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::_;

namespace zephyr {
namespace core {

class HandlerMock {
public:
    MOCK_METHOD1(handle, void(const Message&));
};

TEST(MessageDispatcherTest, CanDeliverMessage) {
    MessageDispatcher dispatcher;
    HandlerMock mock;
    EXPECT_CALL(mock, handle(_));

    using namespace std::placeholders;

    auto callback = std::bind(&HandlerMock::handle, &mock, _1);
    dispatcher.registerHandler(666, callback);
    dispatcher.dispatch({666, 10, std::string("some string")});
}



} /* namespace core */
} /* namespace zephyr */
