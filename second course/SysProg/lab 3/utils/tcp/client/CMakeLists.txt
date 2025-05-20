add_library(tcp_client STATIC
        src/tcp_client.cpp
)
target_include_directories(tcp_client PUBLIC include)