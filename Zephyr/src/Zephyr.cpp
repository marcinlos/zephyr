
#include <zephyr/Root.hpp>
#include <zephyr/demo/MainController.hpp>

using namespace zephyr;
using demo::MainController;

int main(int argc, char* argv[]) try {
    Root root("resources/config.xml");

    MainController demo(root);

    root.run();
    return 0;

} catch (const std::exception& e) {
    std::cerr << "Top-level exception:\n\n" << e.what() << std::endl;
    std::exit(1);
} catch (...) {
    std::cerr << "Unknown exception!" << std::endl;
    std::exit(-1);
}
