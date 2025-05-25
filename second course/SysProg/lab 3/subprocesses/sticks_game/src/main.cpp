#include "message_queue.hpp"

struct GameMessage {
    int sticks_left;
    bool client_turn;
};

int main() {
    MessageQueue mq("/tmp", 'G');
    int sticks = 20;

    while (sticks > 0) {
        int taken;
        mq.receive(&taken, sizeof(taken));
        sticks -= taken;
        GameMessage response{sticks, false};
        mq.send(&response, sizeof(response));
    }
    return 0;
}