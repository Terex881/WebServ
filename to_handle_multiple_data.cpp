#include <iostream>
#include <vector>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>

void handleMultipleFiles(const std::vector<std::string>& filePaths) {
    // Create a pipe to concatenate files
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        std::cerr << "Failed to create pipe" << std::endl;
        exit(1);
    }

    // Fork a child process to concatenate files
    pid_t pid = fork();
    if (pid == -1) {
        std::cerr << "Failed to fork" << std::endl;
        exit(1);
    }

    if (pid == 0) { // Child process: concatenate files
        close(pipefd[0]); // Close read end of the pipe

        for (const auto& filePath : filePaths) {
            int input_fd = open(filePath.c_str(), O_RDONLY);
            if (input_fd == -1) {
                std::cerr << "Failed to open file: " << filePath << std::endl;
                exit(1);
            }

            char buffer[4096];
            ssize_t bytesRead;
            while ((bytesRead = read(input_fd, buffer, sizeof(buffer))) > 0) {
                write(pipefd[1], buffer, bytesRead);
            }

            close(input_fd);
        }

        close(pipefd[1]); // Close write end of the pipe
        exit(0);
    } else { // Parent process: execve
        close(pipefd[1]); // Close write end of the pipe

        // Redirect stdin to read from the pipe
        if (dup2(pipefd[0], STDIN_FILENO) == -1) {
            std::cerr << "Failed to redirect stdin" << std::endl;
            exit(1);
        }

        // Execute the program
        char* args[] = {const_cast<char*>("your_program"), nullptr};
        execve(args[0], args, environ);

        // If execve fails
        std::cerr << "Failed to execute program" << std::endl;
        exit(1);
    }
}

int main() {
    std::vector<std::string> files = {"file1.txt", "file2.txt", "file3.txt"};
    handleMultipleFiles(files);
    return 0;
}