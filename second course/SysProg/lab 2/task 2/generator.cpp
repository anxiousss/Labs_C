#include "generator.hpp"
#include <random>
#include <chrono>
#include <utility>
#include <arpa/inet.h>

LogsGenerator::LogsGenerator(std::unique_ptr<Logger> logger, std::shared_ptr<TsQueue<std::string>> tsQueue, int n_msgs)
        : n_msgs_(n_msgs),
          tsQueue_(std::move(tsQueue)),
          logger_(std::move(logger)){
            if (n_msgs_ < 2) {
                n_msgs_ = 2;
            }
            worker_thread(1);
            if (worker_.joinable()) {worker_.join();}
            ++id;
        }


std::string LogsGenerator::ip_to_string(in_addr_t ip) {
    struct in_addr addr {};
    addr.s_addr = ip;
    return inet_ntoa(addr);
}

void LogsGenerator::worker_thread(int sourceId) {
    std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<> ip_gen(1, 254);
    std::uniform_int_distribution<in_addr_t> ip_dist(0x0A000001, 0x0A0000FF);
    in_addr_t src_ip = ip_dist(gen);
    in_addr_t dst_ip = ip_dist(gen);
    std::uniform_int_distribution<in_port_t> port_dist(1024, 65535);
    in_port_t src_port = port_dist(gen);
    in_port_t dst_port = port_dist(gen);
    std::uniform_int_distribution<size_t> size_dist(64, 4096);
    size_t sz = size_dist(gen);
    std::uniform_int_distribution<size_t> what_pkgs(0, 1);
    std::uniform_int_distribution<size_t> is_fatal(0, 100);
    std::uniform_int_distribution<> delay(500, 1500);


    std::uniform_int_distribution<size_t> pkgs_dist(2, 100);
    traffic.src_addr_ = src_ip;
    traffic.pkgs_sz_ = pkgs_dist(gen);
    try {
        traffic.pkgs = new tcp_traffic_pkg[traffic.pkgs_sz_];
    } catch (const std::exception& e) {
        traffic.src_addr_ = 0;
        traffic.pkgs_sz_ = 0;
        std::cout << "tcp_traffic_pkg bad alloc" << std::endl;
        return;
    }

    tcp_traffic_pkg pkg = {
            .src_port = src_port,
            .dst_addr = dst_ip,
            .dst_port = dst_port,
            .sz = sz
    };

    std::string src_msg = "[" + "Generator " + id + "]" + ip_to_string(src_ip) + ':' + std::to_string(ntohs(sr));

    for (int i = 0; i < n_msgs_; ++i) {
        if (i != 0 && is_fatal(gen) == 0) {

        }
        std::string msg = "Generated " + std::to_string(session.pkgs_sz) +
                          " packets from " + format_ip(src_ip);

        tsQueue_->push(msg);
        std::this_thread::sleep_for(std::chrono::milliseconds(delay(gen)));
    }
}
