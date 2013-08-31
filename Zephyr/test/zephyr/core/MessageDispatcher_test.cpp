/**
 * @file MessageDispatcher_test.cpp
 */

#include <zephyr/core/MessageDispatcher.hpp>
#include <functional>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::_;
using namespace std::placeholders;

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

    registerHandler(dispatcher, 666, &mock, &HandlerMock::handle);
    dispatcher.dispatch({666, 10, std::string("some string")});
}

TEST(MessageDispatcherTest, CanDeliverMessageToMultipleListeners) {
    MessageDispatcher dispatcher;
    HandlerMock firstMock, secondMock;
    EXPECT_CALL(firstMock, handle(_));
    EXPECT_CALL(secondMock, handle(_));

    auto firstCallback = std::bind(&HandlerMock::handle, &firstMock, _1);
    auto secondCallback = std::bind(&HandlerMock::handle, &secondMock, _1);

    dispatcher.registerHandler(666, firstCallback);
    dispatcher.registerHandler(666, secondCallback);
    dispatcher.dispatch({666, 10, std::string("some string")});
}



} /* namespace core */
} /* namespace zephyr */
