/**
 * @file MessageDispatcher_test.cpp
 */

#include <zephyr/core/MessageDispatcher.hpp>
#include <functional>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::_;
using namespace std::placeholders;
using zephyr::util::Any;

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
    Any data { std::string("some string") };
    dispatcher.dispatch({ 666, 10, data });
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
    Any data { std::string("some string") };
    dispatcher.dispatch({ 666, 10, data });
}



} /* namespace core */
} /* namespace zephyr */
