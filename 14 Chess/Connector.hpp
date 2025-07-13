#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unistd.h> // For pipe, fork, exec, read, write, close
#include <sys/wait.h> // For waitpid
#include <fcntl.h> // For fcntl

// File descriptors for pipes
int pipin_w_fd, pipin_r_fd;
int pipout_w_fd, pipout_r_fd;
pid_t child_pid;

void ConnectToEngine(char* path)
{
    int pipe_in[2]; // 0 for read, 1 for write
    int pipe_out[2]; // 0 for read, 1 for write

    if (pipe(pipe_in) == -1 || pipe(pipe_out) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pipin_r_fd = pipe_in[0];
    pipin_w_fd = pipe_in[1];
    pipout_r_fd = pipe_out[0];
    pipout_w_fd = pipe_out[1];

    child_pid = fork();

    if (child_pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (child_pid == 0) { // Child process
        close(pipin_w_fd); // Close write end of child's stdin pipe
        close(pipout_r_fd); // Close read end of child's stdout pipe

        dup2(pipin_r_fd, STDIN_FILENO); // Redirect stdin to read end of pipe_in
        dup2(pipout_w_fd, STDOUT_FILENO); // Redirect stdout to write end of pipe_out
        dup2(pipout_w_fd, STDERR_FILENO); // Redirect stderr to write end of pipe_out

        close(pipin_r_fd); // Close original read end
        close(pipout_w_fd); // Close original write end

        // Execute the chess engine
        char *args[] = {path, NULL};
        execvp(path, args);
        perror("execvp"); // execvp only returns on error
        exit(EXIT_FAILURE);
    } else { // Parent process
        close(pipin_r_fd); // Close read end of parent's stdin pipe
        close(pipout_w_fd); // Close write end of parent's stdout pipe

        // Set pipout_r_fd to non-blocking mode for reading
        int flags = fcntl(pipout_r_fd, F_GETFL, 0);
        if (flags == -1) {
            perror("fcntl F_GETFL");
            exit(EXIT_FAILURE);
        }
        if (fcntl(pipout_r_fd, F_SETFL, flags | O_NONBLOCK) == -1) {
            perror("fcntl F_SETFL O_NONBLOCK");
            exit(EXIT_FAILURE);
        }
    }
}

std::string getNextMove(std::string position)
{
    std::string str;
    position = "position startpos moves " + position + "\ngo\n";

    write(pipin_w_fd, position.c_str(), position.length());
    usleep(500000); // Sleep for 500 milliseconds

    char buffer[2048];
    ssize_t bytes_read;

    // Read available data from the pipe
    while ((bytes_read = read(pipout_r_fd, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytes_read] = '\0';
        str += buffer;
    }

    int n = str.find("bestmove");
    if (n != std::string::npos) return str.substr(n + 9, 4);

    return "error";
}

void CloseConnection()
{
    write(pipin_w_fd, "quit\n", 5);
    close(pipin_w_fd);
    close(pipin_r_fd);
    close(pipout_w_fd);
    close(pipout_r_fd);
    if (child_pid > 0) {
        waitpid(child_pid, NULL, 0); // Wait for the child process to exit
    }
}

#endif // CONNECTOR_H