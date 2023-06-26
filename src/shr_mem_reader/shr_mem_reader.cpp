#include "shr_mem_reader.h"
#include "../Constants.h"

#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <cerrno>
#include <cstring>

void SharedMemoryReader::SetupSharedMemory() {
    ConnectSharedMemory();
    MapMemorySegment();
}

void SharedMemoryReader::PrintContent() {
    GetMessage();
    std::cout << _result << std::endl;
}

void SharedMemoryReader::DeleteSharedMemory() {
    CloseMemorySegment();
    UnlinkMemorySegment();
}

void SharedMemoryReader::ConnectSharedMemory() {
    _shm_fd = shm_open(_name.c_str(), O_RDWR, 0666);

    if (_shm_fd == FAIL_VALUE) {
        throw std::runtime_error("[READER]Connect failed:" + std::string(strerror(errno)));
    }

    ftruncate(_shm_fd, _size);
}

void SharedMemoryReader::MapMemorySegment() {
    _shm_base = (char*)mmap(0, _size, PROT_READ, MAP_SHARED, _shm_fd, 0);

    if (_shm_base == MAP_FAILED) {
        throw std::runtime_error("[READER] Map failed:" + std::string(strerror(errno)));
    }
}

void SharedMemoryReader::GetMessage() {
    _result = std::string(_shm_base, SIZE);
}

void SharedMemoryReader::CloseMemorySegment() {
    if (munmap(_shm_base, _size) == FAIL_VALUE) {
        throw std::runtime_error("[READER] Unmaping faild:" + std::string(strerror(errno)));
    }

    if (close(_shm_fd) == FAIL_VALUE) {
        throw std::runtime_error("[READER] Closing faild:" + std::string(strerror(errno)));
    }
}

void SharedMemoryReader::UnlinkMemorySegment() {
    if (shm_unlink(_name.c_str()) == FAIL_VALUE) {
        throw std::runtime_error("[READER] Error removing " + _name + ": " + std::string(strerror(errno)));
    }
}