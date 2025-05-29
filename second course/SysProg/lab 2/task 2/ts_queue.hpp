#pragma once
#include <vector>
#include <mutex>
#include <condition_variable>
#include <netinet/in.h>

struct tcp_traffic_pkg {
    in_port_t src_port;
    in_addr_t dst_addr;
    in_port_t dst_port;
    size_t sz;
};

struct tcp_traffic {
    in_addr_t src_addr_;
    std::unique_ptr<tcp_traffic_pkg[]> pkgs;
    size_t pkgs_sz_;

    tcp_traffic() : src_addr_(0), pkgs(nullptr), pkgs_sz_(0) {}

    explicit tcp_traffic(size_t size)
            : src_addr_(0), pkgs(std::make_unique<tcp_traffic_pkg[]>(size)), pkgs_sz_(size) {}

    tcp_traffic(tcp_traffic&&) = default;
    tcp_traffic& operator=(tcp_traffic&&) = default;
    tcp_traffic(const tcp_traffic&) = delete;
    tcp_traffic& operator=(const tcp_traffic&) = delete;

    ~tcp_traffic() = default;
};

template <typename T>
class TsQueue : public std::enable_shared_from_this<TsQueue<T>> {
private:
    std::vector<T> data;
    int Ncur = 0, Nrel = 0;
    bool Done{};
    mutable std::mutex mut;
    std::condition_variable cond_consumer, cond_producer;

    bool full() const { return Ncur >= static_cast<int>(data.size()); }
    bool empty() const { return Ncur <= 0; }
    bool done() const { return Done; }

public:
    explicit TsQueue(int size) : data(size) {}

    void push(T element) {
        std::unique_lock<std::mutex> Lk{mut};
        cond_producer.wait(Lk, [this] { return !full(); });
        data[(Ncur + Nrel) % data.size()] = std::move(element);
        ++Ncur;
        Lk.unlock();
        cond_consumer.notify_one();
    }

    bool pop(T &element) {
        std::unique_lock<std::mutex> Lk{mut};
        cond_consumer.wait(Lk, [this] { return !empty() || done(); });
        if (empty()) {
            return false;
        }
        element = std::move(data[Nrel % data.size()]);
        Nrel = (Nrel + 1) % data.size();
        --Ncur;
        Lk.unlock();
        cond_producer.notify_one();
        return true;
    }

    void wake() {
        std::unique_lock<std::mutex> Lk{mut};
        Done = true;
        Lk.unlock();
        cond_consumer.notify_all();
    }

    bool is_empty_done() const {
        std::unique_lock<std::mutex> Lk{mut};
        return (Ncur <= 0) && Done;
    }

    ~TsQueue() = default;
};