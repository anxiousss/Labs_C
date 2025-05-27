#pragma once
#include <string>
#include <memory>
#include <unordered_map>
#include <logger.hpp>
#include <message_queue.hpp>
#include <exceptions.hpp>

struct GameSession {
    int remaining_sticks = 21;
    bool active = true;
};

class GameSubprocess {
public:
    explicit GameSubprocess(key_t key);
    void run();

private:
    key_t key_;
    std::unordered_map<long, GameSession> sessions_;
    std::unique_ptr<MessageQueue> queue_;
    std::unique_ptr<Logger> logger_;

    void process_message(const std::string& message, long type);
    std::string handle_game_turn(int sticks_taken, long session_id);
};