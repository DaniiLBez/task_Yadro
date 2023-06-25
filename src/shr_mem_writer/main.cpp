#include "shr_mem_creator.h"

int main() {
    SharedMemoryConfigBuilder builder;

    std::shared_ptr<SharedMemoryConfig> config = builder.Name("/shm-example").Size(1000).Message("Hello World!");

    auto creator = std::make_shared<SharedMemoryCreator>(config);

    try {
        creator->Create();

        std::cout << config->name << std::endl;

        creator->CloseMemorySegment();
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}