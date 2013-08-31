
#include <zephyr/Root.hpp>

using namespace zephyr;

int main(int argc, char* argv[]) {
    Root root("resources/config.xml");
    root.run();
    return 0;
}
