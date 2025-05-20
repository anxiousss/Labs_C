add_library(logger STATIC
        src/logger.cpp
        include/logger.hpp
)

target_include_directories(logger PUBLIC include)