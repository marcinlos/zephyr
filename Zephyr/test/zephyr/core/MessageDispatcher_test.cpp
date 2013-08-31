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

TEST(MessageDispatcherTest, CanDeliverMessageToMultipleListeners) {
    MessageDispatcher dispatcher;
    HandlerMock firstMock, secondMock;
    EXPECT_CALL(firstMock, handle(_));
    EXPECT_CALL(secondMock, handle(_));

    using namespace std::placeholders;

    auto firstCallback = std::bind(&HandlerMock::handle, &firstMock, _1);
    auto secondCallback = std::bind(&HandlerMock::handle, &secondMock, _1);

    dispatcher.registerHandler(666, firstCallback);
    dispatcher.registerHandler(666, secondCallback);
    dispatcher.dispatch({666, 10, std::string("some string")});
}



} /* namespace core */
} /* namespace zephyr */
