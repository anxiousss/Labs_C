#pragma once

#include <unistd.h>
#include <cstdlib>
#include <string>
#include <iostream>

struct tcp_traffic_pkg {
    size_t sz;
    const char* msg;
};

void interface();

bool check_msg(const std::string& msg);