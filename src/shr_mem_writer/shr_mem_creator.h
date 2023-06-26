#ifndef SHR_MEM_CREATOR_H
#define SHR_MEM_CREATOR_H

#include <memory>
#include <string>

struct SharedMemoryConfig {
    std::size_t size;
    std::string name;
    std::string message;

    SharedMemoryConfig() = default;
};

class SharedMemoryConfigBuilder {
private:
    std::unique_ptr<SharedMemoryConfig> _config;

public:
    SharedMemoryConfigBuilder() {
        _config = std::make_unique<SharedMemoryConfig>();
    }

    SharedMemoryConfigBuilder& Size(const std::size_t Size);

    SharedMemoryConfigBuilder& Name(const std::string& Name);

    SharedMemoryConfigBuilder& Message(const std::string& Message);

    [[nodiscard]] std::unique_ptr<SharedMemoryConfig> Get();

    operator std::unique_ptr<SharedMemoryConfig>() {
        return Get();
    }
};

class SharedMemoryCreator {
    using SharedMemoryConfigPtr = std::unique_ptr<SharedMemoryConfig>;

private:
    SharedMemoryConfigPtr _config;

    std::size_t _shm_fd;
    char* _shm_base;

public:
    explicit SharedMemoryCreator(SharedMemoryConfigPtr Config)
        : _config(std::move(Config)) {}

    SharedMemoryConfig& getConfig() const;

    void Create();

    std::size_t SharedMemoryFD() const;

    char* SharedMemoryBase() const;

    void CloseMemorySegment();

private:
    void CreateMemorySegment();

    void MapMemorySegment();

    void SetMessage();
};

#endif  // SHR_MEM_CREATOR_H