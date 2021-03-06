/**
 * @file MessageQueue_test.cpp
 */

#include <zephyr/core/MessageQueue.hpp>
#include <zephyr/core/Message.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::AtLeast;
using zephyr::util::Any;

namespace zephyr {
namespace core {

const char* text = "meh";

TEST(MessageQueueTest, EmptyQueueIsEmpty) {
    MessageQueue queue;
    EXPECT_TRUE(queue.empty());
}

TEST(MessageQueueTest, NotEmptyAfterPost) {
    MessageQueue queue;
    Message message { 10, 7, Any { text } };
    queue.post(message);
    EXPECT_FALSE(queue.empty());
}

TEST(MessageQueueTest, PopWithEmptyThrows) {
    MessageQueue queue;
    EXPECT_THROW(queue.pop(), std::runtime_error);
}

TEST(MessageQueueTest, CanFetchPostedMessage) {
    MessageQueue queue;
    Message message { 10, 7, Any { text } };
    queue.post(message);

    Message popped = queue.pop();
    EXPECT_EQ(message.target, popped.target);
    EXPECT_EQ(message.type, popped.type);
}


} /* namespace core */
} /* namespace zephyr */
