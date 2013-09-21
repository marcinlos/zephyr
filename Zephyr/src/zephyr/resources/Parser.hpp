/**
 * @file Parser.hpp
 */

#ifndef ZEPHYR_RESOURCES_PARSER_HPP_
#define ZEPHYR_RESOURCES_PARSER_HPP_

#include <zephyr/resources/ast.hpp>
#include <iterator>
#include <boost/property_tree/ptree.hpp>


namespace zephyr {
namespace resources {

class Parser {
public:

    void parse(std::istream& input);

    void parse(const std::string& path);

    ast::Root collectAst() {
        return std::move(defs);
    }

private:

    void updateWith(const boost::property_tree::ptree& tree);

    ast::Root defs;

};

} /* namespace resources */
} /* namespace zephyr */

#endif /* ZEPHYR_RESOURCES_PARSER_HPP_ */
