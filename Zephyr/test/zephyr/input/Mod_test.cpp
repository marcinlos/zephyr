/**
 * @file Mod_test.cpp
 */

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <boost/lexical_cast.hpp>
#include <zephyr/input/Mod.hpp>

namespace zephyr {
namespace input {

TEST(ModTest, EmptyModifierResultsInEmptyString) {
    EXPECT_EQ("none", boost::lexical_cast<std::string>(Mod::NONE));
}

TEST(ModTest, OneItemIsHandledCorrectly) {
    EXPECT_EQ("ctrl", boost::lexical_cast<std::string>(Mod::CTRL));
}

TEST(ModTest, TwoItemsAreHandledCorrectly) {
    Mod val = Mod::CTRL | Mod::SHIFT;
    EXPECT_EQ("ctrl|shift", boost::lexical_cast<std::string>(val));
}

TEST(ModTest, ThreeItemsAreHandledCorrestly) {
    Mod val = Mod::CTRL | Mod::SHIFT | Mod::ALT;
    EXPECT_EQ("ctrl|alt|shift", boost::lexical_cast<std::string>(val));
}


} /* namespace input */
} /* namespace zephyr */
