#ifndef SHR_MEM_READER_H
#define SHR_MEM_READER_H

#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <errno.h>
#include <string.h>

#define FAIL_VALUE -1
#define SEGMENT_SIZE 1000

class SharedMemoryReader {
private:
    std::string _result;
    std::string _name;
    int32_t _size;

    int32_t _shm_fd;
    char* _shm_base;

public:

    SharedMemoryReader(const std::string& Name, const int32_t Size)
        : _name(Name)
        , _size(Size) {}

    void SetupSharedMemory() {
        ConnectSharedMemory();
        MapMemorySegment();
    }

    void PrintContent() {
        GetMessage();
        std::cout << _result << std::endl;
    }

    void DeleteSharedMemory() {
        CloseMemorySegment();
        UnlinkMemorySegment();
    }

private:
    void ConnectSharedMemory() {
        _shm_fd = shm_open(_name.c_str(), O_RDWR, 0666);

        if (_shm_fd == FAIL_VALUE) {
            throw std::runtime_error("[READER]Connect failed:" + std::string(strerror(errno)));
        }

        ftruncate(_shm_fd, _size);
    }

    void MapMemorySegment() {
        _shm_base = (char*)mmap(0, _size, PROT_READ, MAP_SHARED, _shm_fd, 0);

        if (_shm_base == MAP_FAILED) {
            throw std::runtime_error("[READER] Map failed:" + std::string(strerror(errno)));
        }
    }

    void GetMessage() {
        _result = std::string(_shm_base, SEGMENT_SIZE);
    }

    void CloseMemorySegment() {
        if (munmap(_shm_base, _size) == FAIL_VALUE) {
            throw std::runtime_error("[READER] Unmaping faild:" + std::string(strerror(errno)));
        }

        if (close(_shm_fd) == FAIL_VALUE) {
            throw std::runtime_error("[READER] Closing faild:" + std::string(strerror(errno)));
        }
    }

    void UnlinkMemorySegment() {
        if (shm_unlink(_name.c_str()) == FAIL_VALUE) {
            throw std::runtime_error("[READER] Error removing " + _name + ": " + std::string(strerror(errno)));
        }
    }
};

#endif
