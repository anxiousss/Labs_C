#pragma once
#include <map>
#include <mutex>
#include <atomic>
#include <thread>
#include <memory>
#include <netinet/in.h>
#include "../task1/logger.hpp"
#include "ts_queue.hpp"

class LogAnalyzer {
public:
    struct IpStats {
        size_t sent_bytes = 0;
        size_t received_bytes = 0;
        size_t sent_packets = 0;
        size_t received_packets = 0;
        size_t connections = 0;
        std::map<in_port_t, size_t> dst_ports;
    };

    LogAnalyzer(std::shared_ptr<TsQueue<tcp_traffic>> queue, std::unique_ptr<Logger> logger);
    ~LogAnalyzer();

    void start();
    void stop();
    void join();

    std::map<in_addr_t, IpStats> get_stats() const;
    IpStats get_ip_stats(in_addr_t ip) const;

private:
    std::map<in_addr_t, IpStats> local_stats_;
    mutable std::mutex stats_mutex_;
    std::shared_ptr<TsQueue<tcp_traffic>> queue_;
    std::unique_ptr<Logger> logger_;
    std::thread worker_;
    std::atomic<bool> running_{false};

    void process_queue();
};

class AnalyzerStatsCollector {
public:
    static std::map<in_addr_t, LogAnalyzer::IpStats> collect_stats(
            const std::vector<std::unique_ptr<LogAnalyzer>>& analyzers);

    static void print_stats(const std::map<in_addr_t, LogAnalyzer::IpStats>& stats);
};