#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>     // For fork() and execvp()
#include <sys/types.h>  // For pid_t
#include <sys/wait.h>   // For waitpid
#include <time.h>       // For clock_gettime()

#define MAX_ARGS 128 // Maximum number of arguments per command

int main() {
    char input[256]; // Buffer for user input

    // Display welcome message
    printf("Welcome to ENSEA Tiny Shell.\n");
    printf("Type 'exit' to quit.\n");

    while (1) {
        // Display prompt
        printf("enseash %% ");

        // Read user input
        if (fgets(input, sizeof(input), stdin) == NULL) {
            // Handle end of file (Ctrl+D)
            printf("Bye bye...\n");
            break;
        }
        
        // Remove trailing newline character from input
        input[strcspn(input, "\n")] = '\0';

        // Exit the shell if the user types 'exit'
        if (strcmp(input, "exit") == 0) {
            printf("Bye bye...\n");
            break;
        }

        // Parse the command and arguments
        char *args[MAX_ARGS];
        char *token = strtok(input, " ");
        int i = 0;
        
         // Tokenize the input into command and arguments
        while (token != NULL && i < MAX_ARGS - 1) {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL; // Null-terminate the arguments array

        // Fork a new process to execute the command
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork failed");
            continue;
        }
        
        int status;
        struct timespec start, end; // For measuring execution time

        if (pid == 0) {
            // In child process: start time measurement
            clock_gettime(CLOCK_MONOTONIC, &start);

            // Execute the command with arguments
            if (execvp(args[0], args) == -1) {
                perror("Command execution failed");
                exit(EXIT_FAILURE);
            }
         } else {
            // In parent process: wait for the child to finish
            waitpid(pid, &status, 0);

            // End time measurement after child finishes
            clock_gettime(CLOCK_MONOTONIC, &end);

            // Calculate elapsed time in milliseconds
            long seconds = end.tv_sec - start.tv_sec;

            // Check the exit status of the child process
            if (WIFEXITED(status)) {
                // Child exited normally
                printf("enseash [exit:%d|%ldms] %% ", WEXITSTATUS(status), seconds); // Exit code and execution time
            } else if (WIFSIGNALED(status)) {
				// Child was terminated by a signal
                printf("enseash [sign:%d|%ldms] %% ", WTERMSIG(status), seconds); // Signal number and execution time
            }
        }
    }
    
    return 0;
}
