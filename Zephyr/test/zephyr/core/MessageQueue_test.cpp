/*
 * MessageQueue_test.cpp
 *
 *  Created on: Aug 29, 2013
 *      Author: los
 */

#include <zephyr/core/MessageQueue.hpp>
#include <zephyr/core/Message.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::AtLeast;

namespace zephyr {
namespace core {

TEST(MessageQueueTest, EmptyQueueIsEmpty) {
    MessageQueue queue;
    EXPECT_TRUE(queue.empty());
}

TEST(MessageQueueTest, NotEmptyAfterPost) {
    MessageQueue queue;
    queue.post({ 10, 7, "meh" });
    EXPECT_FALSE(queue.empty());
}

TEST(MessageQueueTest, PopWithEmptyThrows) {
    MessageQueue queue;
    EXPECT_THROW(queue.pop(), std::runtime_error);
}

TEST(MessageQueueTest, CanFetchPostedMessage) {
    MessageQueue queue;
    Message message { 10, 7, "meh" };
    queue.post(message);

    Message popped = queue.pop();
    EXPECT_EQ(message.target, popped.target);
    EXPECT_EQ(message.type, popped.type);
}

} /* namespace core */
} /* namespace zephyr */
