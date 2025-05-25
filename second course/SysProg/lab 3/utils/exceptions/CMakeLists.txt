add_library(exceptions STATIC
        include/exceptions.hpp
        src/exceptions.cpp
)
target_include_directories(exceptions PUBLIC include)