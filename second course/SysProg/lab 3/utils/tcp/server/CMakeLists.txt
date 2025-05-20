add_library(tcp_server STATIC
        src/tcp_server.cpp
)

target_include_directories(tcp_server PUBLIC include)