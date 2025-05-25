#include "message_queue.hpp"

MessageQueue::MessageQueue(const std::string& path, int proj_id) {
    key = ftok(path.c_str(), proj_id);
    if (key == -1) throw std::runtime_error("ftok failed for MessageQueue");

    msg_id = msgget(key, IPC_CREAT | 0666);
    if (msg_id == -1) throw std::runtime_error("msgget failed");
}

MessageQueue::~MessageQueue() {
    // Очередь сообщений обычно не удаляется автоматически
}

void MessageQueue::send(const void* msg, size_t size, long mtype) {
    struct msgbuf {
        long mtype;
        char data[1024];
    } buf;

    buf.mtype = mtype;
    memcpy(buf.data, msg, size);

    if (msgsnd(msg_id, &buf, size, 0) == -1)
        throw std::runtime_error("msgsnd failed");
}

void MessageQueue::receive(void* msg, size_t size, long mtype) {
    struct msgbuf {
        long mtype;
        char data[1024];
    } buf;

    if (msgrcv(msg_id, &buf, size, mtype, 0) == -1)
        throw std::runtime_error("msgrcv failed");

    memcpy(msg, buf.data, size);
}