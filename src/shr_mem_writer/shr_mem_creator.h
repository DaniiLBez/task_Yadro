#ifndef SHR_MEM_CREATOR_H
#define SHR_MEM_CREATOR_H

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>
#include <memory>
#include <string>

#define FAIL_VALUE -1

struct SharedMemoryConfig {
    int32_t size;
    std::string name;
    std::string message;

    SharedMemoryConfig() = default;

    SharedMemoryConfig(SharedMemoryConfig* config) {
        this->size = config->size;
        this->name = config->name;
        this->message = config->message;
    }
};

class SharedMemoryConfigBuilder {
private:
    SharedMemoryConfig* _config;

public:
    SharedMemoryConfigBuilder()
        : _config(new SharedMemoryConfig()) {}

    SharedMemoryConfigBuilder& Size(const int32_t& Size) {
        this->_config->size = Size;
        return *this;
    }

    SharedMemoryConfigBuilder& Name(const std::string& Name) {
        _config->name = Name;
        return *this;
    }

    SharedMemoryConfigBuilder& Message(const std::string& Message) {
        _config->message += Message;
        return *this;
    }

    std::shared_ptr<SharedMemoryConfig> Get() const {
        auto memoryConfig = std::shared_ptr<SharedMemoryConfig>(new SharedMemoryConfig(_config));
        delete this->_config;
        return memoryConfig;
    }

    operator std::shared_ptr<SharedMemoryConfig>() {
        return Get();
    }
};

class SharedMemoryCreator {
    using SharedMemoryConfigPtr = std::shared_ptr<SharedMemoryConfig>;

private:
    SharedMemoryConfigPtr _config;

    int32_t _shm_fd;
    char* _shm_base;

public:
    SharedMemoryCreator(const SharedMemoryConfigPtr& Config)
        : _config(std::move(Config)) {}

    void Create() {
        CreateMemorySegment();
        MapMemorySegment();
        SetMessage();
    }

    int SharedMemoryFD() const {
        return _shm_fd;
    }

    char* SharedMemoryBase() const {
        return _shm_base;
    }

    void CloseMemorySegment() {
        if (munmap(_shm_base, _config->size) == FAIL_VALUE) {
            throw std::runtime_error("[WRITER] Unmaping faild:" + std::string(strerror(errno)));
        }

        if (close(_shm_fd) == FAIL_VALUE) {
            throw std::runtime_error("[WRITER] Closing faild:" + std::string(strerror(errno)));
        }
    }

private:
    void CreateMemorySegment() {
        _shm_fd = shm_open(_config->name.c_str(), O_CREAT | O_RDWR, 0777);

        if (_shm_fd == FAIL_VALUE) {
            throw std::runtime_error("[WRITER]Create failed:" + std::string(strerror(errno)));
        }

        ftruncate(_shm_fd, _config->size);
    }

    void MapMemorySegment() {
        _shm_base = (char*)mmap(0, _config->size, PROT_READ | PROT_WRITE, MAP_SHARED, _shm_fd, 0);

        if (_shm_base == MAP_FAILED) {
            throw std::runtime_error("[WRITER] Map failed:" + std::string(strerror(errno)));
        }
    }

    void SetMessage() {
        sprintf(_shm_base, "%s", _config->message.c_str());
    }
};

#endif  // SHR_MEM_CREATOR_H