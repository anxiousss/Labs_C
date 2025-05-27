add_library(exceptions STATIC
        include/exceptions.hpp
        include/tcp_client_exceptions.hpp
        include/tcp_server_exception.hpp
        src/exceptions.cpp
        src/tcp_client_exceptions.cpp
        src/tcp_server_exceptions.cpp)
target_include_directories(exceptions PUBLIC include)
target_link_libraries(exceptions PRIVATE logger)