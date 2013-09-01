/**
 * @file Any_test.cpp
 */

#include <zephyr/util/Any.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <string>
#include <sstream>


#include <type_traits>

namespace zephyr {
namespace util {

std::string to_string(const Any& any) {
    std::ostringstream ss;
    ss << any;
    return ss.str();
}

struct can_stream { };

struct no_stream { };

std::ostream& operator << (std::ostream& os, const can_stream&) {
    return os << "content";
}

std::ostream&& f() {
    return std::move(std::cout);
}

void sth() {
//    f() << no_stream{};
    // why is rvalue reference ok in this unevaluated context?
//    sizeof(std::declval<std::ostream>() << std::declval<no_stream>());
//    int j = sizeof(std::declval<std::ostream&>() << std::declval<no_stream>());
}


TEST(AnyTest, CanPrintInt) {
    EXPECT_EQ("12", to_string(Any(12)));
}

TEST(AnyTest, CanPrintStructWithOverloadedStream) {
    EXPECT_EQ("content", to_string(Any(can_stream {})));
}

TEST(AnyTest, CannotPrintStructWithNoOverloadedStream) {

#ifdef __GLIBC__
    const char* expected = "zephyr::util::no_stream";
#else
    const char* expected = "[non-printable]";
#endif

    EXPECT_EQ(expected, to_string(Any(no_stream { })));
}

TEST(AnyTest, CanGetValue) {
    Any any(666);
    EXPECT_EQ(666, any_cast<int>(any));
}

TEST(AnyTest, CanGetConstValue) {
    const Any any(666);
    EXPECT_EQ(666, any_cast<int>(any));
}

TEST(AnyTest, CanGetPtrValue) {
    Any any(666);
    EXPECT_EQ(666, *any_cast<int>(&any));
}

TEST(AnyTest, CanGetConstPtrValue) {
    const Any any(666);
    EXPECT_EQ(666, *any_cast<int>(&any));
}


} /* namespace util */
} /* namespace zephyr */

