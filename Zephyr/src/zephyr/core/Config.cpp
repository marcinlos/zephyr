#include <zephyr/core/Config.hpp>
#include <boost/property_tree/xml_parser.hpp>

namespace zephyr {
namespace core {

using boost::property_tree::xml_parser::read_xml;

void Config::loadXML(const std::string& path) {
    read_xml(path, properties);
}

void Config::loadXML(std::istream& stream) {
    read_xml(stream, properties);
}

const Config::node& Config::getNode(const std::string& path) const {
    return properties.get_child(path);
}

} /* namespace core */
} /* namespace zephyr */
