#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

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
        
        if (pid == 0) {
            // In child process: execute the command
            char *args[] = {input, NULL}; // Command without arguments
            if (execvp(input, args) == -1) {
                perror("Command execution failed");
                exit(EXIT_FAILURE);
            }
        } else {
            // In parent process: wait for the child to finish
            int status;
            waitpid(pid, &status, 0);
        }
    }

       return 0;
}
