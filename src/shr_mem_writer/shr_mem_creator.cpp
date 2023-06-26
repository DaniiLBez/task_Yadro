#include "shr_mem_creator.h"
#include "Constants.h"

#include <stdexcept>
#include <cerrno>
#include <fcntl.h>
#include <cstring>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>

SharedMemoryConfigBuilder& SharedMemoryConfigBuilder::Size(const size_t Size) {
    this->_config->size = Size;
    return *this;
}

SharedMemoryConfigBuilder& SharedMemoryConfigBuilder::Name(const std::string& Name) {
    _config->name = Name;
    return *this;
}

SharedMemoryConfigBuilder& SharedMemoryConfigBuilder::Message(const std::string& Message) {
    _config->message += Message;
    return *this;
}

std::unique_ptr<SharedMemoryConfig> SharedMemoryConfigBuilder::Get() {
    auto memoryConfig = std::make_unique<SharedMemoryConfig>();
    memoryConfig.swap(_config);
    return memoryConfig;
}

SharedMemoryConfig& SharedMemoryCreator::getConfig() const {
    return *_config;
}

void SharedMemoryCreator::Create() {
    CreateMemorySegment();
    MapMemorySegment();
    SetMessage();
}

std::size_t SharedMemoryCreator::SharedMemoryFD() const {
    return _shm_fd;
}

char* SharedMemoryCreator::SharedMemoryBase() const {
    return _shm_base;
}

void SharedMemoryCreator::CloseMemorySegment() {
    if (munmap(_shm_base, _config->size) == FAIL_VALUE) {
        throw std::runtime_error("[WRITER] Unmaping faild:" + std::string(strerror(errno)));
    }

    if (close(_shm_fd) == FAIL_VALUE) {
        throw std::runtime_error("[WRITER] Closing faild:" + std::string(strerror(errno)));
    }
}

void SharedMemoryCreator::CreateMemorySegment() {
    _shm_fd = shm_open(_config->name.c_str(), O_CREAT | O_RDWR, 0777);

    if (_shm_fd == FAIL_VALUE) {
        throw std::runtime_error("[WRITER]Create failed:" + std::string(strerror(errno)));
    }

    ftruncate(_shm_fd, _config->size);
}

void SharedMemoryCreator::MapMemorySegment() {
    _shm_base = (char*)mmap(0, _config->size, PROT_READ | PROT_WRITE, MAP_SHARED, _shm_fd, 0);

    if (_shm_base == MAP_FAILED) {
        throw std::runtime_error("[WRITER] Map failed:" + std::string(strerror(errno)));
    }
}

void SharedMemoryCreator::SetMessage() {
    sprintf(_shm_base, "%s", _config->message.c_str());
}
