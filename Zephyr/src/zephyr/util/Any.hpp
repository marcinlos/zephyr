/**
 * @file Any.hpp
 */

#ifndef ZEPHYR_UTIL_ANY_HPP_
#define ZEPHYR_UTIL_ANY_HPP_

#include <iostream>
#include <memory>
#include <type_traits>
#include <zephyr/util/make_unique.hpp>
#include <boost/any.hpp>

#ifdef __GLIBC__
    #include <cxxabi.h>
#endif /* __GLIBC__ */


namespace zephyr {
namespace util {


struct printer {

    /** Prints the data contained in boost::any to the stream */
    virtual void print(std::ostream& os, const boost::any& data) = 0;

    /** Polymorphically copies itself */
    virtual std::unique_ptr<printer> clone() = 0;

    virtual ~printer() = default;
};

namespace {

template <typename T>
const char* get_name() {

#ifdef __GLIBC__
    int status;
    return abi::__cxa_demangle(typeid(T).name(), nullptr, nullptr, &status);
#else
    return "[non-printable]";
#endif /* __GLIBC__ */

}


template <typename T>
struct is_printable_helper {

    template <typename U>
    static std::true_type test(
        char(*)[sizeof(std::declval<std::ostream&>() << std::declval<U>())]
    );

    template <typename U>
    static std::false_type test(...);

    typedef decltype(test<T>(nullptr)) type;

};

template <typename T>
struct is_printable: is_printable_helper<T>::type {};

template <typename T>
void print_value(std::ostream& os, const T& value, std::true_type) {
    os << value;
}

template <typename T>
void print_value(std::ostream& os, const T&, std::false_type) {
    os << get_name<T>();
}


template <typename T>
void print_value(std::ostream& os, const T& value) {
    print_value(os, value, typename is_printable<T>::type());
}


template <typename T>
struct printer_impl: printer {

    void print(std::ostream& os, const boost::any& data) override {
        print_value(os, boost::any_cast<T>(data));
    }

    std::unique_ptr<printer> clone() override {
        return make_unique<printer_impl<T>>();
    }

};

} /* namespace */


class Any {
public:

    Any() = default;

    template <typename T>
    Any(const T& value)
    : printer_(make_unique<printer_impl<T>>())
    , data(value)
    { }

    Any(const Any& any)
    : printer_(any.printer_ ? any.printer_->clone() : nullptr)
    , data(any.data)
    { }

    Any(Any&&) = default;

    template <typename T>
    Any& operator = (const T& value) {
        Any any(value);
        swap(*this, any);
        return *this;
    }

    Any& operator = (Any other) {
        swap(*this, other);
        return *this;
    }

    friend std::ostream& operator << (std::ostream& os, const Any& any) {
        if (any.printer_) {
            any.printer_->print(os, any.data);
        } else {
            os << "[empty]";
        }
        return os;
    }

private:
    std::unique_ptr<printer> printer_;
    boost::any data;

    friend void swap(Any& left, Any& right) {
        using std::swap;

        swap(left.printer_, right.printer_);
        swap(left.data, right.data);
    }

};


} /* namespace util */
} /* namespace zephyr */


#endif /* ZEPHYR_UTIL_ANY_HPP_ */
