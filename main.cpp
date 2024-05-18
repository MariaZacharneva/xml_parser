#include <iostream>

#include "cmake-build-debug/Controller.h"

int main(int argc, char *argv[]) {
    std::cout << "Hello, user!" << std::endl;
    if (argc >= 3) {
        std::cout << "Using custom parameters..." << std::endl;
        auto controller = std::make_unique<Controller>(argv[1], argv[2]);
        controller->Run();
    } else {
        std::cout << "Using default parameters..." << std::endl;
        auto controller = std::make_unique<Controller>("test/test.xml", "test");
        controller->Run();
    }
    return 0;
}
