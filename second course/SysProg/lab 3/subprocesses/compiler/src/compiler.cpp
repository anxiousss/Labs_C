#include "compiler.hpp"

CompilerSubprocess::CompilerSubprocess(const std::string& queue_name)
        : queue_name_(queue_name), logger_(LoggerBuilder("Compiler").set_console().build()) {
    logger_->LogInfo("Compiler subprocess initialized with queue: " + queue_name_);
}

void CompilerSubprocess::run() {
    try {
        SharedMemoryManager shm(queue_name_);

        while (true) {
            try {
                SharedMemoryTask task = shm.receive();
                logger_->LogInfo("Received task for file: " + task.filename);
                process_task(task);
            } catch (const ExceptionBase& e) {
                e.log(logger_);
            }
        }
    } catch (const ExceptionBase& e) {
        e.log(logger_);
    }
}

void CompilerSubprocess::process_task(const SharedMemoryTask& task) {
    try {
        std::string output_file;

        if (task.extension == ".cpp") {
            std::string command = "g++ " + task.filename + " -o " + task.filename + ".out";
            logger_->LogInfo("Executing command: " + command);

            int result = system(command.c_str());
            if (result == -1) {
                throw std::runtime_error("Failed to execute g++");
            }
            output_file = task.filename + ".out";
        } else if (task.extension == ".tex") {
            std::string command = "pdflatex " + task.filename;
            logger_->LogInfo("Executing command: " + command);

            int result = system(command.c_str());
            if (result == -1) {
                throw std::runtime_error("Failed to execute pdflatex");
            }
            output_file = task.filename + ".pdf";
        } else {
            logger_->LogWarning("Unsupported file type: " + task.extension);
            return;
        }

        logger_->LogInfo("Compilation completed. Output file: " + output_file);
        // Compilation
    } catch (const std::exception& e) {
        logger_->LogError("Compilation error: " + std::string(e.what()));
    }
}