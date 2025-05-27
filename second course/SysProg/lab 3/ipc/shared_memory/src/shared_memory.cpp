#include <cstring>
#include "shared_memory.hpp"

SharedMemoryTask SharedMemoryManager::receive() {
    if (ptr_ == MAP_FAILED) {
        throw std::runtime_error(".")
    }

    char* data = static_cast<char*>(ptr_);
    size_t offset = 0;

    // Чтение данных
    size_t filename_size = *reinterpret_cast<size_t*>(data + offset);
    offset += sizeof(size_t);

    std::string filename(data + offset, filename_size);
    offset += filename_size;

    size_t extension_size = *reinterpret_cast<size_t*>(data + offset);
    offset += sizeof(size_t);

    std::string extension(data + offset, extension_size);
    offset += extension_size;

    size_t content_size = *reinterpret_cast<size_t*>(data + offset);
    offset += sizeof(size_t);

    std::string content(data + offset, content_size);

    return {filename, content, extension, "", false};
}

void SharedMemoryManager::send(const SharedMemoryTask& task) {
    if (ptr_ == MAP_FAILED) {
        throw std::runtime_error(".")
    }

    char* data = static_cast<char*>(ptr_);
    size_t offset = 0;

    size_t filename_size = task.filename.size();
    *reinterpret_cast<size_t*>(data + offset) = filename_size;
    offset += sizeof(size_t);

    std::memcpy(data + offset, task.filename.data(), filename_size);
    offset += filename_size;

    size_t extension_size = task.extension.size();
    *reinterpret_cast<size_t*>(data + offset) = extension_size;
    offset += sizeof(size_t);

    std::memcpy(data + offset, task.extension.data(), extension_size);
    offset += extension_size;

    size_t content_size = task.content.size();
    *reinterpret_cast<size_t*>(data + offset) = content_size;
    offset += sizeof(size_t);

    std::memcpy(data + offset, task.content.data(), content_size);
    offset += content_size;

    size_t result_size = task.result.size();
    *reinterpret_cast<size_t*>(data + offset) = result_size;
    offset += sizeof(size_t);

    std::memcpy(data + offset, task.result.data(), result_size);
}