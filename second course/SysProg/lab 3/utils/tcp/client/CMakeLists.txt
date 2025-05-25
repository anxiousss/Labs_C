add_library(tcp_client
        include/tcp_client.hpp
        src/tcp_client.cpp
)

target_link_libraries(tcp_client
        PRIVATE logger
        PRIVATE exceptions)

target_include_directories(tcp_client PUBLIC include)