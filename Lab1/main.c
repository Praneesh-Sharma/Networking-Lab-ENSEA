#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

#define MAX_ARGS 128
#define INPUT_BUFFER_SIZE 256

typedef struct {
    char input[INPUT_BUFFER_SIZE];
} Shell;

void display_welcome_message();
void display_prompt();
void execute_command(Shell *shell);
void run_shell(Shell *shell);
void measure_execution_time(struct timespec *start, struct timespec *end, long *elapsed_ms);

void display_welcome_message() {
    printf("Welcome to ENSEA Tiny Shell.\n");
    printf("Type 'exit' to quit.\n");
}

void display_prompt() {
    printf("enseash %% ");
}

void execute_command(Shell *shell) {
    char *args[MAX_ARGS];
    char *token = strtok(shell->input, " ");
    int i = 0;
    
     while (token != NULL && i < MAX_ARGS - 1) {
        args[i++] = token;
        token = strtok(NULL, " ");
    }
    args[i] = NULL; 
    
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork failed");
        return;
    }
    
    struct timespec start, end;
    if (pid == 0) {
        if (execvp(args[0], args) == -1) {
            perror("Command execution failed");
            exit(EXIT_FAILURE);
        }
    } else {
        clock_gettime(CLOCK_MONOTONIC, &start);
        int status;
        waitpid(pid, &status, 0);
        clock_gettime(CLOCK_MONOTONIC, &end);
		
        long elapsed_ms;
        measure_execution_time(&start, &end, &elapsed_ms);

        if (WIFEXITED(status)) {
            printf("enseash [exit:%d|%ldms] %% ", WEXITSTATUS(status), elapsed_ms);
        } else if (WIFSIGNALED(status)) {
            printf("enseash [sign:%d|%ldms] %% ", WTERMSIG(status), elapsed_ms);
        }
    }
}

void measure_execution_time(struct timespec *start, struct timespec *end, long *elapsed_ms) {
    long seconds = end->tv_sec - start->tv_sec;
    long nanoseconds = end->tv_nsec - start->tv_nsec;
    *elapsed_ms = seconds * 1000 + nanoseconds / 1000000;
}

void run_shell(Shell *shell) {
    display_welcome_message();

    while (1) {
        display_prompt();
        
        if (fgets(shell->input, INPUT_BUFFER_SIZE, stdin) == NULL) {
            printf("Bye bye...\n");
            break;
        }
        
        shell->input[strcspn(shell->input, "\n")] = '\0';

        if (strcmp(shell->input, "exit") == 0) {
            printf("Bye bye...\n");
            break;
        }
        
        execute_command(shell);
    }
}

int main() {
    Shell shell;
    run_shell(&shell);
    return 0;
}
