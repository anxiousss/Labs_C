#include "shared_memory.hpp"
#include "semaphore.hpp"
#include "tcp_server.hpp"
#include <cstdlib>
#include <filesystem>
#include <cstring>

namespace fs = std::filesystem;

// Структура для результатов компиляции
struct CompileResult {
    int client_socket;
    bool success;
    char output_file[256];
};

int main() {
    try {
        // IPC для получения задач
        SharedMemory task_shm("/tmp", 'C', sizeof(CompileTask));
        Semaphore task_sem("/tmp", 'S');

        // IPC для отправки результатов
        MessageQueue result_mq("/tmp", 'R');

        while(true) {
            // Ожидаем новую задачу
            task_sem.wait();

            // Читаем задачу из разделяемой памяти
            CompileTask task;
            std::memcpy(&task, task_shm.getData(), sizeof(task));

            // Выполняем компиляцию
            std::string command;
            std::string output_file;
            bool success = false;

            if(task.type == CompileTask::Type::CPP) {
                output_file = std::string(task.file_path) + ".exe";
                command = "g++ " + std::string(task.file_path) + " -o " + output_file;
            } else {
                output_file = std::string(task.file_path) + ".pdf";
                command = "pdflatex " + std::string(task.file_path);
            }

            int result = std::system(command.c_str());
            success = (result == 0);

            // Формируем результат
            CompileResult compile_result{};
            compile_result.client_socket = task.client_socket;
            compile_result.success = success;
            std::strncpy(
                    compile_result.output_file,
                    output_file.c_str(),
                    sizeof(compile_result.output_file)
            );

            // Отправляем результат через очередь сообщений
            result_mq.send(&compile_result, sizeof(compile_result));

            // Удаляем исходный файл
            fs::remove(task.file_path);
        }
    } catch(const std::exception& e) {
        std::cerr << "Compiler error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}