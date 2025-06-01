add_executable(lab2task2 ../task1/logger.cpp ../task1/logger.hpp analyzer.hpp analyzer.cpp
                generator.hpp generator.cpp ts_queue.hpp ts_queue.cpp main.cpp)

add_link_options(-pthread)