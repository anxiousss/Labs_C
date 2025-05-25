add_library(message_queue STATIC
        src/message_queue.cpp
)
target_include_directories(message_queue PUBLIC include)