add_library(semaphore STATIC
        include/semaphore.hpp
        src/semaphore.cpp
)

target_link_libraries(semaphore
        PRIVATE logger
        PRIVATE exceptions)


target_include_directories(semaphore PUBLIC include)