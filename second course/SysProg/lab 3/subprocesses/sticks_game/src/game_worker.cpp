#include "game_worker.hpp"

GameSubprocess::GameSubprocess(key_t key)
        : key_(key), logger_(LoggerBuilder("GameServer").set_console().build()) {
    queue_ = std::make_unique<MessageQueue>(key_);
    logger_->LogInfo("Game subprocess initialized with key: " + std::to_string(key_));
}

void GameSubprocess::run() {
    logger_->LogInfo("Game server running...");

    while (true) {
        try {
            std::string message = queue_->receive();
            // Format: <session_id>:<sticks_taken>
            size_t colon_pos = message.find(':');
            if (colon_pos == std::string::npos) {
                logger_->LogWarning("Invalid message format: " + message);
                continue;
            }

            long session_id = std::stol(message.substr(0, colon_pos));
            int sticks_taken = std::stoi(message.substr(colon_pos + 1));

            process_message(message, session_id);
        } catch (const std::exception& e) {
            logger_->LogError("Error processing message: " + std::string(e.what()));
        }
    }
}

void GameSubprocess::process_message(const std::string& message, long session_id) {
    auto it = sessions_.find(session_id);
    if (it == sessions_.end()) {
        // New game session
        sessions_[session_id] = GameSession{};
        logger_->LogInfo("Started new game session with ID: " + std::to_string(session_id));
    }

    GameSession& session = sessions_[session_id];

    if (!session.active) {
        queue_->send("Game already finished", session_id);
        return;
    }

    if (sticks_taken < 1 || sticks_taken > 3) {
        queue_->send("You can only take 1-3 sticks", session_id);
        return;
    }

    if (session.remaining_sticks - sticks_taken < 0) {
        queue_->send("Not enough sticks to take", session_id);
        return;
    }

    session.remaining_sticks -= sticks_taken;

    if (session.remaining_sticks == 0) {
        session.active = false;
        queue_->send("You took the last stick. You lose!", session_id);
        return;
    }

    // Server's turn - simple AI that takes 1-3 sticks
    int server_take = (session.remaining_sticks - 1) % 4;
    if (server_take == 0) server_take = 1;

    session.remaining_sticks -= server_take;

    if (session.remaining_sticks == 0) {
        session.active = false;
        queue_->send("Server took " + std::to_string(server_take) +
                     " sticks. You win!", session_id);
    } else {
        queue_->send("Server took " + std::to_string(server_take) +
                     " sticks. Remaining: " + std::to_string(session.remaining_sticks),
                     session_id);
    }
}