/**
 * @file input_adapter_test.cpp
 */

#include <zephyr/glfw/input_adapter.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using zephyr::input::Mod;

namespace zephyr {
namespace glfw {

TEST(InputAdapterTest, EmptyModResultsInNone) {
    EXPECT_EQ(Mod::NONE, modFromGLFW(0));
}

TEST(InputAdapterTest, ShiftResultsInModShift) {
    EXPECT_EQ(Mod::SHIFT, modFromGLFW(GLFW_MOD_SHIFT));
}

TEST(InputAdapterTest, CtrlWithAltResultsInModShift) {
    int val = GLFW_MOD_CONTROL | GLFW_MOD_ALT;
    EXPECT_EQ(Mod::CTRL | Mod::ALT, modFromGLFW(val));
}

} /* namespace glfw */
} /* namespace zephyr */
