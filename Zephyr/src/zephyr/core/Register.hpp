/**
 * @file Register.hpp
 */

#ifndef ZEPHYR_CORE_REGISTER_HPP_
#define ZEPHYR_CORE_REGISTER_HPP_

#include <zephyr/util/Any.hpp>
#include <zephyr/util/format.hpp>
#include <unordered_map>
#include <string>

using zephyr::util::Any;

namespace zephyr {
namespace core {

class Register {
public:

    Any& operator [] (const std::string& var) {
        auto it = vars_.find(var);
        if (it != end(vars_)) {
            return it->second;
        } else {
            throw std::runtime_error(util::format("No variable '{}'", var));
        }
    }

    const Any& operator [] (const std::string& var) const {
        return const_cast<Register&>(*this)[var];
    }

    bool exists(const std::string& var) const {
        return vars_.find(var) != end(vars_);
    }

    template <typename T>
    T& get(const std::string& var) {
        return util::any_cast<T>((*this)[var]);
    }

    template <typename T>
    const T& get(const std::string& var) const {
        return util::any_cast<T>((*this)[var]);
    }

    template <typename T>
    void put(std::string var, T value) {
        vars_.insert({{ std::move(var), util::Any(std::move(value)) }});
    }

private:
    std::unordered_map<std::string, Any> vars_;
};


} /* namespace core */
} /* namespace zephyr */

#endif /* ZEPHYR_CORE_REGISTER_HPP_ */
