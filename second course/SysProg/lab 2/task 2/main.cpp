#include <iostream>
#include <vector>
#include <memory>
#include <thread>
#include <map>
#include <mutex>
#include <atomic>
#include <chrono>
#include <stdexcept>
#include <netinet/in.h>
#include "../task1/logger.hpp"
#include "ts_queue.hpp"
#include "generator.hpp"
#include "analyzer.hpp"

int main() {
    try {
        auto logger = LoggerBuilder("main")
                .set_level(log_lvl::INFO)
                .set_console()
                .build();

        auto queue = std::make_shared<TsQueue<tcp_traffic>>(100);

        const int NUM_GENERATORS = 5;
        const int NUM_ANALYZERS = 3;

        std::vector<std::unique_ptr<LogsGenerator>> generators;
        std::vector<std::unique_ptr<LogAnalyzer>> analyzers;

        for (int i = 0; i < NUM_GENERATORS; ++i) {
            auto generator_logger = LoggerBuilder("generator_logger_" + std::to_string(i))
                    .set_level(log_lvl::INFO)
                    .set_console()
                    .build();

            generators.emplace_back(std::make_unique<LogsGenerator>(
                    std::move(generator_logger), queue, 10));
        }

        for (int i = 0; i < NUM_ANALYZERS; ++i) {
            auto analyzer_logger = LoggerBuilder("analyzer_logger_" + std::to_string(i))
                    .set_level(log_lvl::INFO)
                    .set_console()
                    .build();

            analyzers.emplace_back(std::make_unique<LogAnalyzer>(
                    queue, std::move(analyzer_logger)));
            analyzers.back()->start();
        }

        for (auto& gen : generators) {
            gen->join();
        }

        queue->wake();

        for (auto& ana : analyzers) {
            ana->join();
        }

        auto global_stats = AnalyzerStatsCollector::collect_stats(analyzers);

        std::cout << "\n=== Глобальная статистика по IP-адресам ===\n";
        for (const auto& [ip, stats] : global_stats) {
            std::cout << "IP: " << LogsGenerator::ip_to_string(ip) << "\n"
                      << "  Отправлено: " << stats.sent_bytes << " байт\n"
                      << "  Получено: " << stats.received_bytes << " байт\n"
                      << "  Соединений: " << stats.connections << "\n"
                      << "  Порты назначения:\n";

            for (const auto& [port, count] : stats.dst_ports) {
                std::cout << "    Порт " << ntohs(port) << ": " << count << " раз\n";
            }
            std::cout << "-------------------------\n";
        }

        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return 1;
    }
}