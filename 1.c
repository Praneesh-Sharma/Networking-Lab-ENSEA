#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

        // Exit the shell if user types 'exit'
        if (strcmp(input, "exit") == 0) {
            printf("Goodbye!\n");
            break;
        }

        // If input is not 'exit', simply echo it (for now)
        printf("You entered: %s\n", input);
    }
       return 0;
}
