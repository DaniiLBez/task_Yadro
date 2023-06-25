#include "shr_mem_reader.h"

int main() {
    std::string mem_segment_name;
    std::cin >> mem_segment_name;

    auto reader = new SharedMemoryReader(mem_segment_name, SEGMENT_SIZE);

    try {
        reader->SetupSharedMemory();
        reader->PrintContent();
        reader->DeleteSharedMemory();
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}