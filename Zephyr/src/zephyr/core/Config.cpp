
#include <zephyr/core/Config.hpp>
#include <boost/property_tree/xml_parser.hpp>

namespace zephyr
{
namespace core
{

using boost::property_tree::xml_parser::read_xml;


void Config::loadXML(const std::string& path)
{
    read_xml(path, properties);
}

void Config::loadXML(std::istream& stream)
{
    read_xml(stream, properties);
}

} /* namespace core */
} /* namespace zephyr */
