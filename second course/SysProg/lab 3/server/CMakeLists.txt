add_executable(server
        include/server.hpp
        src/main.cpp
        src/server.cpp
)


target_include_directories(server PRIVATE include)

target_link_libraries(server
        PRIVATE tcp_server
        PRIVATE logger
)