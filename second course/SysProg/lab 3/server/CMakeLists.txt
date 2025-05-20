add_executable(server
        src/main.cpp
        src/server.cpp
        include/server.hpp
        ../cmake-build-debug/utils/logger/liblogger.a
)

target_include_directories(server PRIVATE include ../utils)
target_link_libraries(server PRIVATE tcp_server tcp_client logger)