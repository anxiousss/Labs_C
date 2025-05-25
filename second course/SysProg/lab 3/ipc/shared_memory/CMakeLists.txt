add_library(shared_memory STATIC
        src/shared_memory.cpp
)
target_include_directories(shared_memory PUBLIC include)