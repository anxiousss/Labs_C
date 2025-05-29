#include "analyzer.hpp"
#include <mutex>

LogAnalyzer::LogAnalyzer(std::shared_ptr<TsQueue<tcp_traffic>> queue,
                         std::unique_ptr<Logger> logger)
        : queue_(std::move(queue)), logger_(std::move(logger)) {}

LogAnalyzer::~LogAnalyzer() {
    if (worker_.joinable()) {
        worker_.join();
    }
}

void LogAnalyzer::start() {
    running_ = true;
    worker_ = std::thread(&LogAnalyzer::process_queue, this);
}

void LogAnalyzer::stop() {
    running_ = false;
    queue_->wake();
}

void LogAnalyzer::join() {
    if (worker_.joinable()) {
        worker_.join();
    }
}

void LogAnalyzer::process_queue() {
    tcp_traffic traffic;
    while (running_.load() || !queue_->is_empty_done()) {
        if (!queue_->pop(traffic)) continue;

        {
            std::lock_guard<std::mutex> lock(stats_mutex_);
            local_stats_[traffic.src_addr_].connections++;

            for (size_t i = 0; i < traffic.pkgs_sz_; ++i) {
                const auto& pkg = traffic.pkgs[i];
                local_stats_[traffic.src_addr_].sent_bytes += pkg.sz;
                local_stats_[traffic.src_addr_].dst_ports[pkg.dst_port]++;
                local_stats_[pkg.dst_addr].received_bytes += pkg.sz;
            }
        }
    }
}

std::map<in_addr_t, LogAnalyzer::IpStats> LogAnalyzer::get_stats() const {
    std::lock_guard<std::mutex> lock(stats_mutex_);
    return local_stats_;
}

LogAnalyzer::IpStats LogAnalyzer::get_ip_stats(in_addr_t ip) const {
    std::lock_guard<std::mutex> lock(stats_mutex_);
    auto it = local_stats_.find(ip);
    return (it != local_stats_.end()) ? it->second : IpStats{};
}

std::map<in_addr_t, LogAnalyzer::IpStats>
AnalyzerStatsCollector::collect_stats(const std::vector<std::unique_ptr<LogAnalyzer>>& analyzers) {
    std::map<in_addr_t, LogAnalyzer::IpStats> global_stats;

    for (const auto& analyzer : analyzers) {
        auto local = analyzer->get_stats();
        for (const auto& [ip, stats] : local) {
            global_stats[ip].sent_bytes += stats.sent_bytes;
            global_stats[ip].received_bytes += stats.received_bytes;
            global_stats[ip].connections += stats.connections;

            for (const auto& [port, count] : stats.dst_ports) {
                global_stats[ip].dst_ports[port] += count;
            }
        }
    }

    return global_stats;
}