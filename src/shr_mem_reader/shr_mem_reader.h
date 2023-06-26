#ifndef SHR_MEM_READER_H
#define SHR_MEM_READER_H

#include <string>

class SharedMemoryReader {
private:
    std::string _result;
    std::string _name;
    std::size_t _size;

    std::size_t _shm_fd;
    char* _shm_base;

public:

    SharedMemoryReader(const std::string& Name, const size_t Size)
        : _name(Name)
        , _size(Size) {}

    void SetupSharedMemory();

    void PrintContent();

    void DeleteSharedMemory();

private:
    void ConnectSharedMemory();

    void MapMemorySegment();

    void GetMessage();

    void CloseMemorySegment();

    void UnlinkMemorySegment();
};

#endif
