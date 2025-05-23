add_library(tcp_server
        src/tcp_server.cpp
)

target_link_libraries(tcp_server
        PRIVATE logger)

target_include_directories(tcp_server PUBLIC include)