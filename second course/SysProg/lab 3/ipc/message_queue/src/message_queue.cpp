#include "message_queue.hpp"

MessageQueue::MessageQueue(key_t key)
        : key_(key), msqid_(-1), logger_(LoggerBuilder("MessageQueue").set_console().build()) {
    msqid_ = msgget(key_, 0666 | IPC_CREAT);
    if (msqid_ == -1) {
        throw std::system_error(errno, std::generic_category(), "msgget failed");
    }
    logger_->LogInfo("Created message queue with key: " + std::to_string(key_));
}

MessageQueue::~MessageQueue() {
    close();

    if (msgctl(msqid_, IPC_RMID, nullptr) == -1) {
        logger_->LogError("msgctl IPC_RMID failed: " + std::string(strerror(errno)));
    }
}

void MessageQueue::send(const std::string& message, long type) {
    if (msqid_ == -1) {
        throw std::runtime_error("Message queue not initialized");
    }

    MessageBuffer buffer;
    buffer.mtype = type;
    strncpy(buffer.mtext, message.c_str(), sizeof(buffer.mtext));
    buffer.mtext[sizeof(buffer.mtext) - 1] = '\0';

    if ( msgsnd(msqid_, &buffer, message.size() + 1, 0) == -1) {
        throw std::system_error(errno, std::generic_category(), "msgsnd failed");
    }
    logger_->LogDebug("Sent message: " + message);
}

std::string MessageQueue::receive(long type) {
    if (msqid_ == -1) {
        throw std::runtime_error("Message queue not initialized");
    }

    MessageBuffer buffer;
    ssize_t bytes_received = msgrcv(msqid_, &buffer, sizeof(buffer.mtext), type, 0);

    if (bytes_received == -1) {
        throw std::system_error(errno, std::generic_category(), "msgrcv failed");
    }

    logger_->LogDebug("Received message: " + std::string(buffer.mtext));
    return std::string(buffer.mtext);
}

void MessageQueue::close() {

}