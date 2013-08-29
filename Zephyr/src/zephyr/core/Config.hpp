/**
 * @file Config.hpp
 */

#ifndef ZEPHYR_CORE_CONFIG_HPP_
#define ZEPHYR_CORE_CONFIG_HPP_

#include <string>
#include <istream>
#include <boost/property_tree/ptree.hpp>

namespace zephyr
{
namespace core
{

/**
 * Generic configuration manager. Supports reading and writing configuration
 * using files or arbitrary streams.
 */
class Config
{
public:

    /**
     * Loads XML-encoded configuration from file.
     *
     * @param [in] path Path denoting the file to load
     */
    void loadXML(const std::string& path);

    /**
     * Loads XML-encoded configuration from arbitrary @c istream
     *
     * @param [in] stream Stream to read configuration from
     */
    void loadXML(std::istream& stream);

    /** Retrieves the value specified by the path */
    template <typename T>
    T get(const std::string& path) const;

    /**
     * Retrieves the value specified by the path, using @c def as a fallback
     * default value.
     *
     * @param [in] path Path denoting the property, e.g.
     */
    template <typename T>
    T get(const std::string& path, const T& def) const;

private:
    /** Tree storing configuration data */
    boost::property_tree::ptree properties;
};

template <typename T>
T Config::get(const std::string& path) const
{
    return properties.get<T>(path);
}

template <typename T>
T Config::get(const std::string& path, const T& def) const
{
    return properties.get<T>(path, def);
}

} /* namespace core */
} /* namespace zephyr */

#endif /* ZEPHYR_CORE_CONFIG_HPP_ */
