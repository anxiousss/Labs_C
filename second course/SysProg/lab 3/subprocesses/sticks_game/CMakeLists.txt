add_executable(game_worker
        src/main.cpp
        src/game_worker.cpp
)

target_include_directories(game_worker PRIVATE include)
target_link_libraries(game_worker PRIVATE tcp_server logger exceptions message_queue pthread)