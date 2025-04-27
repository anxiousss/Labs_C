
#pragma once

#include <atomic>
#include <vector>
#include <thread>
#include <mutex>
#include <map>
#include <netinet/in.h>
#include "../task1/logger.hpp"
#include "ts_queue.hpp"

struct tcp_traffic_pkg {
    in_port_t src_port;
    in_addr_t dst_addr;
    in_port_t dst_port;
    size_t sz;
};

struct tcp_traffic {
    in_addr_t src_addr_;
    tcp_traffic_pkg* pkgs;
    size_t pkgs_sz_;

    tcp_traffic(): src_addr_(0), pkgs(nullptr), pkgs_sz_(0) {}

    ~tcp_traffic() {
        delete[] pkgs;
    }
};

class LogsGenerator {
public:
    LogsGenerator(std::unique_ptr<Logger> logger, std::shared_ptr<TsQueue<std::string>> tsQueue, int n_msgs);
    ~LogsGenerator() = default;

private:
    struct SessionStats {
        size_t total_bytes = 0;
        size_t packet_count = 0;
    };
    int n_msgs_;
    std::shared_ptr<TsQueue<std::string>> tsQueue_;
    std::unique_ptr<Logger> logger_;
    std::thread worker_;
    tcp_traffic traffic;
    inline static int id = 1;

    mutable std::mutex stats_mutex_;
    std::map<in_addr_t, SessionStats> stats_;

    std::string ip_to_string(in_addr_t ip);

    void worker_thread(int sourceId);
};