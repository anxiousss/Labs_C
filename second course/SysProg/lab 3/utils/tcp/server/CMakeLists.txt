add_library(tcp_server
        include/tcp_server.hpp
        src/tcp_server.cpp
)

target_link_libraries(tcp_server
        PRIVATE logger
        PRIVATE exceptions
        PRIVATE shared_memory
        PRIVATE semaphore
        PRIVATE message_queue)

target_include_directories(tcp_server PUBLIC include)