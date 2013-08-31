/**
 * @file DispatcherTask_test.cpp
 */

#include <zephyr/core/DispatcherTask.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::_;
using ::testing::Return;

namespace zephyr {
namespace core {

class DispatcherMock: public MessageDispatcher {
public:
    MOCK_METHOD1(dispatch, void (const Message&));
};

struct DispatcherTaskTest: testing::Test {

    MessageQueue queue;
    DispatcherMock dispatcher;
    DispatcherTask task;

    DispatcherTaskTest()
    : task(queue, dispatcher)
    { }

};

TEST_F(DispatcherTaskTest, DoesNothingForEmptyQueue) {
    EXPECT_CALL(dispatcher, dispatch(_)).Times(0);
    task.update();
}

TEST_F(DispatcherTaskTest, DeliversSingleMessage) {
    queue.post({10, 666, 7});
    EXPECT_CALL(dispatcher, dispatch(_)).Times(1);
    task.update();
}


TEST_F(DispatcherTaskTest, DeliversAllMessages) {
    queue.post({10, 666, 7});
    queue.post({17, 63, 12});
    queue.post({4, 66, 67});
    EXPECT_CALL(dispatcher, dispatch(_)).Times(3);
    task.update();
}

} /* namespace core */
} /* namespace zephyr */
