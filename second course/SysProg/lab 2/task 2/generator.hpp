#pragma once
#include <atomic>
#include <vector>
#include <thread>
#include <mutex>
#include <map>
#include <netinet/in.h>
#include "../task1/logger.hpp"
#include "ts_queue.hpp"

class LogsGenerator {
public:
    LogsGenerator(std::unique_ptr<Logger> logger,
                  std::shared_ptr<TsQueue<tcp_traffic>> tsQueue,
                  int n_msgs);
    ~LogsGenerator();
    void join();
    static std::string ip_to_string(in_addr_t ip);


private:
    struct SessionStats {
        size_t total_bytes = 0;
        size_t packet_count = 0;
    };

    int n_msgs_;
    std::shared_ptr<TsQueue<tcp_traffic>> tsQueue_;
    std::unique_ptr<Logger> logger_;
    std::thread worker_;
    inline static int id = 1;
    mutable std::mutex stats_mutex_;
    std::map<in_addr_t, SessionStats> stats_;
    void worker_thread();
};