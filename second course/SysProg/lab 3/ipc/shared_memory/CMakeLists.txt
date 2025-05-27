add_library(shared_memory STATIC
        src/shared_memory.cpp
        include/shared_memory.hpp
)

target_link_libraries(shared_memory
                PRIVATE logger
                PRIVATE exceptions)

target_include_directories(shared_memory PUBLIC include)