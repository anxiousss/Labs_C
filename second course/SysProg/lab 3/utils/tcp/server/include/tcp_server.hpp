#pragma once

#include <string>
#include <thread>

class Task {
public:
    Task();
    virtual int complete_task() = 0;

private:
    int id;
    std::string msg;
    std::thread thread;

};

class TaskCompile: public Task {
public:
    int complete_task() override {
        //logic
        return 0;
    }
};

class TaskStick: public Task {
public:
    int complete_task() override {
        //logic
        return 0;
    }
};