#include "shr_mem_creator.h"
#include "../Constants.h"

#include <iostream>

int main() {
    SharedMemoryConfigBuilder builder;

    std::unique_ptr<SharedMemoryConfig> config = builder.Name(NAME).Size(SIZE).Message(MESSAGE);

    auto creator = std::make_unique<SharedMemoryCreator>(std::move(config));

    try {
        creator->Create();

        std::cout << creator->getConfig().name << std::endl;

        creator->CloseMemorySegment();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}