#include "generator.hpp"
#include <random>
#include <chrono>
#include <utility>
#include <arpa/inet.h>

LogsGenerator::LogsGenerator(std::unique_ptr<Logger> logger,
                             std::shared_ptr<TsQueue<tcp_traffic>> tsQueue,
                             int n_msgs)
        : tsQueue_(std::move(tsQueue)),
          logger_(std::move(logger)) {
    n_msgs_ = (n_msgs < 2) ? 2 : n_msgs;
    worker_ = std::thread(&LogsGenerator::worker_thread, this);
    ++id;
}

LogsGenerator::~LogsGenerator() {
    if (worker_.joinable()) {
        worker_.join();
    }
}

void LogsGenerator::join() {
    if (worker_.joinable()) {
        worker_.join();
    }
}

std::string LogsGenerator::ip_to_string(in_addr_t ip) {
    struct in_addr addr {};
    addr.s_addr = ip;
    return inet_ntoa(addr);
}

void LogsGenerator::worker_thread() {
    std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<in_addr_t> ip_dist(0x0A000001, 0x0A0000FE);
    in_addr_t src_ip = ip_dist(gen);
    in_addr_t dst_ip = ip_dist(gen);
    std::uniform_int_distribution<in_port_t> port_dist(1024, 65535);
    in_port_t src_port = port_dist(gen);
    in_port_t dst_port = port_dist(gen);
    std::uniform_int_distribution<size_t> size_dist(64, 4096);
    std::uniform_int_distribution<> is_fatal(0, 99);
    std::uniform_int_distribution<> delay(500, 1500);

    std::string src_ip_str = ip_to_string(src_ip);
    std::string dst_ip_str = ip_to_string(dst_ip);
    std::string src_port_str = std::to_string(ntohs(src_port));
    std::string dst_port_str = std::to_string(ntohs(dst_port));

    std::string connect_msg = "Generator " + std::to_string(id) + " CONNECT " +
                              src_ip_str + ":" + src_port_str + " → " + dst_ip_str + ":" + dst_port_str;
    logger_->LogInfo(connect_msg);

    tcp_traffic traffic(n_msgs_ - 2);
    traffic.src_addr_ = src_ip;

    for (size_t i = 0; i < traffic.pkgs_sz_; ++i) {
        auto& pkg = traffic.pkgs[i];
        pkg.src_port = src_port;
        pkg.dst_addr = dst_ip;
        pkg.dst_port = dst_port;
        pkg.sz = size_dist(gen);

        std::string type = (i % 2 == 0) ? "GET" : "POST";
        std::string data_msg = "Generator " + std::to_string(id) + " " + type + " " +
                               src_ip_str + ":" + src_port_str + " → " + dst_ip_str + ":" + dst_port_str +
                               " (" + std::to_string(pkg.sz) + ")";
        logger_->LogInfo(data_msg);
        std::this_thread::sleep_for(std::chrono::milliseconds(delay(gen)));
    }

    tsQueue_->push(std::move(traffic));

    bool unexpected = (is_fatal(gen) < 10);
    std::string disconnect_msg = "Generator " + std::to_string(id) + " DISCONNECT " +
                                 src_ip_str + ":" + src_port_str + " → " + dst_ip_str + ":" + dst_port_str;
    if (unexpected) {
        logger_->LogError(disconnect_msg + " (unexpected)");
    } else {
        logger_->LogInfo(disconnect_msg);
    }
}