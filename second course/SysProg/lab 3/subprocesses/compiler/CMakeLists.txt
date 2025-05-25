add_executable(compiler
        src/main.cpp
        src/compiler.cpp
)

target_include_directories(compiler PRIVATE include)
target_link_libraries(compiler PRIVATE tcp_server logger exceptions shared_memory semaphore pthread)