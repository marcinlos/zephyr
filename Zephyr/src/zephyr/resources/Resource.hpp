/**
 * @file Resource.hpp
 */

#ifndef ZEPHYR_RESOURCES_RESOURCE_HPP_
#define ZEPHYR_RESOURCES_RESOURCE_HPP_

#include <memory>


namespace zephyr {
namespace resources {


class Resource: public std::enable_shared_from_this<Resource> {
public:

    virtual void load() = 0;

    virtual void unload() = 0;

    virtual ~Resource() = default;

};

typedef std::shared_ptr<Resource> ResourcePtr;


} /* namespace resources */
} /* namespace zephyr */

#endif /* ZEPHYR_RESOURCES_RESOURCE_HPP_ */
