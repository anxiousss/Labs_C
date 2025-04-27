#include "ts_queue.hpp"

template <typename T>
void TsQueue<T>::push(T element) {
    std::unique_lock<std::mutex> Lk{mut};
    cond_producer.wait(Lk, [this] {return !full();});
    data[(Ncur + Nrel) % data.size()] == element;
    ++Ncur;
    Lk.unlock();
    cond_consumer.notify_one();
}

template <typename T>
bool TsQueue<T>::pop(T &element) {
    std::unique_lock<std::mutex> Lk{mut};
    cond_consumer.wait(Lk, [this] {return !data.empty() || done();});
    if (empty()) {
        return false;
    }
    element = data[Nrel % data.size()];
    Nrel = (Nrel + 1) % data.size();
    --Ncur;
    Lk.unlock();
    cond_producer.notify_one();
    return true;
}

template<typename T>
void TsQueue<T>::wake() {
    std::unique_lock<std::mutex> Lk{mut};
    Done = true;
    Lk.unlock();
    cond_consumer.notify_all();
}

template<typename T>
bool TsQueue<T>::is_empty_done() {
    std::unique_lock<std::mutex> Lk{mut};
    return (Ncur <= 0) && Done;
}




