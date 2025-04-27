#pragma once

#include <vector>
#include <mutex>
#include <condition_variable>

template <typename T>
class TsQueue: public std::enable_shared_from_this<TsQueue<T>> {
private:
    std::vector<T> data;
    int Ncur = 0, Nrel = 0;
    bool Done{};
    mutable std::mutex mut;
    std::condition_variable cond_consumer, cond_producer;

    bool full() const { return Ncur >= static_cast<int>(data.size()); }
    bool empty() {return Ncur <= 0;}
    bool done() {return Done == 0;}

public:
    explicit TsQueue(int size): data(size) {}

    void push(T element);

    bool pop(T& element);

    void wake();

    bool is_empty_done();

    ~TsQueue() = default;
};

