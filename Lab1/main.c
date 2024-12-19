#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

#define MAX_ARGS 128
#define INPUT_BUFFER_SIZE 256

// Shell structure definition
typedef struct {
    char input[INPUT_BUFFER_SIZE];  // Buffer for user input
} Shell;

// Function prototypes
void display_welcome_message();
void display_prompt();
void execute_command(Shell *shell);
void run_shell(Shell *shell);
void measure_execution_time(struct timespec *start, struct timespec *end, long *elapsed_ms);

// Function to display the welcome message
void display_welcome_message() {
    printf("Welcome to ENSEA Tiny Shell.\n");
    printf("Type 'exit' to quit.\n");
}

// Function to display the prompt
void display_prompt() {
    printf("enseash %% ");
}

// Function to execute a command
void execute_command(Shell *shell) {
    // Parse the command and arguments
    char *args[MAX_ARGS];
    char *token = strtok(shell->input, " ");
    int i = 0;
    
     while (token != NULL && i < MAX_ARGS - 1) {
        args[i++] = token;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;  // Null-terminate the arguments array
    
    // Fork a new process
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork failed");
        return;
    }
    
    struct timespec start, end;  // For measuring execution time
    if (pid == 0) {
        // Child process: execute the command
        if (execvp(args[0], args) == -1) {
            perror("Command execution failed");
            exit(EXIT_FAILURE);
        }
    } else {
		// Parent process: measure execution time and wait for the child
        clock_gettime(CLOCK_MONOTONIC, &start);
        int status;
        waitpid(pid, &status, 0);
        clock_gettime(CLOCK_MONOTONIC, &end);
		
		// Calculate elapsed time
        long elapsed_ms;
        measure_execution_time(&start, &end, &elapsed_ms);

		 // Check the status of the child process
        if (WIFEXITED(status)) {
            printf("enseash [exit:%d|%ldms] %% ", WEXITSTATUS(status), elapsed_ms);
        } else if (WIFSIGNALED(status)) {
            printf("enseash [sign:%d|%ldms] %% ", WTERMSIG(status), elapsed_ms);
        }
    }
}

// Function to measure execution time
void measure_execution_time(struct timespec *start, struct timespec *end, long *elapsed_ms) {
    long seconds = end->tv_sec - start->tv_sec;
    long nanoseconds = end->tv_nsec - start->tv_nsec;
    *elapsed_ms = seconds * 1000 + nanoseconds / 1000000;
}

// Function to run the shell
void run_shell(Shell *shell) {
    display_welcome_message();

    while (1) {
        display_prompt();
        
        // Read user input
        if (fgets(shell->input, INPUT_BUFFER_SIZE, stdin) == NULL) {
            printf("Bye bye...\n");
            break;
        }
        
        // Remove trailing newline character
        shell->input[strcspn(shell->input, "\n")] = '\0';

        // Handle 'exit' command
        if (strcmp(shell->input, "exit") == 0) {
            printf("Bye bye...\n");
            break;
        }
        
         // Execute the command
        execute_command(shell);
    }
}

// Main function
int main() {
    Shell shell;  // Instantiate the Shell object
    run_shell(&shell);
    return 0;
}
