#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> // For fork() and execvp()
#include <sys/types.h> // For pid_t
#include <sys/wait.h> // For waitpid
#include <time.h>       // For clock_gettime()

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
            printf("\nGoodbye!\n");
            break;
        }
    
		// Remove trailing newline character from input
        input[strcspn(input, "\n")] = '\0';

        // Exit the shell if the user types 'exit'
        if (strcmp(input, "exit") == 0) {
            printf("Bye bye...\n");
            break;
        }
        
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

            // Execute the command
            char *args[] = {input, NULL}; // Command without arguments
            if (execvp(input, args) == -1) {
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
            long nanoseconds = end.tv_nsec - start.tv_nsec;
            if (nanoseconds < 0) {
                seconds--;
                nanoseconds += 1000000000;
            }
             long elapsed_ms = (seconds * 1000) + (nanoseconds / 1000000);

            // Check the exit status of the child process
            if (WIFEXITED(status)) {
				// Child exited normally
                printf("enseash [exit:%d|%ldms] %% ", WEXITSTATUS(status), elapsed_ms); // Exit code and execution time
            } else if (WIFSIGNALED(status)) {
                // Child was terminated by a signal
                printf("enseash [sign:%d|%ldms] %% ", WTERMSIG(status), elapsed_ms); // Signal number and execution time
            }
		}
    }

       return 0;
}
