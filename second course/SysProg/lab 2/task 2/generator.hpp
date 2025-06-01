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


private:
    struct SessionStats {
        size_t total_bytes = 0;
        size_t packet_count = 0;
    };

    int n_msgs_;
    std::shared_ptr<TsQueue<tcp_traffic>> tsQueue_;
    std::unique_ptr<Logger> logger_;
    std::thread worker_;
    int my_id_;
    mutable std::mutex stats_mutex_;
    std::map<in_addr_t, SessionStats> stats_;
    inline static std::atomic<int> next_id{1};
    void worker_thread();
};