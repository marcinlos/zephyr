/**
 * @file ResourceManager.hpp
 */

#ifndef ZEPHYR_RESOURCES_RESOURCEMANAGER_HPP_
#define ZEPHYR_RESOURCES_RESOURCEMANAGER_HPP_


namespace zephyr {
namespace resources {

/**
 * Default loading strategy - when an unknown resource is encountered,
 * exception is thrown.
 */
template <typename T>
struct NoLoader {

    T operator ()(const std::string& name) {
        throw std::runtime_error("No such resource");
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

    T& put(const std::string& name, const T& value) {
        return resources[name] = value;
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
