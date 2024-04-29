#include <iostream>

#include "cmake-build-debug/Controller.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
    auto controller = std::make_unique<Controller>("test.xml");
    controller->Run();
    return 0;
}
