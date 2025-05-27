add_library(message_queue STATIC
        include/message_queue.hpp
        src/message_queue.cpp
)

target_link_libraries(message_queue
        PRIVATE logger
        PRIVATE exceptions)
target_include_directories(message_queue PUBLIC include)