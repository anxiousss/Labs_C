add_library(tcp_server
        include/tcp_server.hpp
        src/tcp_server.cpp
)

target_link_libraries(tcp_server
        PUBLIC shared_memory
        PUBLIC logger
        PUBLIC exceptions
        PUBLIC semaphore
        PUBLIC message_queue)

target_include_directories(tcp_server PUBLIC include)