/**
 * @file ResourceManager.hpp
 */

#ifndef ZEPHYR_RESOURCES_RESOURCEMANAGER_HPP_
#define ZEPHYR_RESOURCES_RESOURCEMANAGER_HPP_

#include <zephyr/util/format.hpp>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <boost/optional.hpp>



namespace zephyr {
namespace resources {

template <typename T>
using Maybe = boost::optional<T>;

template <typename T>
Maybe<T> indeed(const T& val) {
    return boost::make_optional(val);
}

constexpr boost::none_t nothing = boost::none;


/**
 * Default loading strategy - when an unknown resource is encountered,
 * exception is thrown.
 */
template <typename T>
struct NoLoader {

    T operator ()(const std::string& name) {
        throw std::runtime_error(util::format("No such resource: {}", name));
    }

};

/**
 * Simple, map-based resource manager template.
 */
template <typename T, typename Loader = NoLoader<T>>
class ResourceManager {
private:

    struct Proxy {
        ResourceManager& manager;
        std::string name;

        Proxy(ResourceManager& manager, std::string name)
        : manager(manager)
        , name(std::move(name))
        { }

        operator T& () {
            return manager.doGet(name);
        }

        Proxy& operator = (const T& value) {
            manager.put(name, value);
            return *this;
        }
    };

public:

    ResourceManager(Loader loader = { })
    : loader(loader)
    { }

    T& put(std::string name, const T& value) {
        return resources[std::move(name)] = value;
    }

    const T& get(const std::string& name) const {
        return const_cast<ResourceManager&>(*this).doGet(name);
    }

    const T& operator [](const std::string& name) const {
        return get(name);
    }

    Proxy operator [](std::string name) {
        return Proxy(*this, std::move(name));
    }

    bool exists(const std::string& name) {
        auto it = resources.find(name);
        return it != end(resources);
    }

    Maybe<T> tryGet(const std::string& name) {
        auto it = resources.find(name);
        if (it != end(resources)) {
            return indeed(it->second);
        } else {
            return nothing;
        }
    }

private:

    T& doGet(const std::string& name) {
        auto result = resources.find(name);
        if (result != end(resources)) {
            return result->second;
        } else {
            T value = loader(name);
            return put(name, value);
        }
    }

    std::unordered_map<std::string, T> resources;
    Loader loader;
};


} /* namespace resources */
} /* namespace zephyr */


#endif /* ZEPHYR_RESOURCES_RESOURCEMANAGER_HPP_ */
