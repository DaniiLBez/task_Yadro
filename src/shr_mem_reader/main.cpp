#include "shr_mem_reader.h"
#include "../Constants.h"

#include <iostream>
#include <memory>

int main() {
    std::string mem_segment_name;
    std::cin >> mem_segment_name;

    auto reader = std::make_unique<SharedMemoryReader>(mem_segment_name, SIZE);

    try {
        reader->SetupSharedMemory();
        reader->PrintContent();
        reader->DeleteSharedMemory();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}