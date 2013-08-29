#include <iostream>

#include <gmock/gmock.h>

TEST(are_not_equal, test_equality)
{
    EXPECT_NE(2, 1);
}