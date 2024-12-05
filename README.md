# Networking-Lab-ENSEA




# ENSEA Tiny Shell (enseash)
This project is a tiny shell implementation called `enseash`, developed as part of the
**Synthesis Practical Work** for the 2nd year of Computer Science at ENSEA. The shell
provides functionality to execute commands, display exit codes and execution times, and
manage advanced features like input/output redirection, piping, and background
processes.

## Features Implemented
1. **Welcome Message**: Displays a welcome message when the shell starts.
 Example:
 ```
 Welcome to ENSEA Tiny Shell.
 Type 'exit' to quit.
 enseash %
 ```
2. **Command Execution (REPL)**: Reads, evaluates, and executes simple commands.
 Example:
 ```
 enseash % date
 Sun Dec 13 13:19:40 CET 2020
 enseash %
 ```
3. **Exit Management**: Handles the `exit` command or `<Ctrl>+D` to terminate the shell
gracefully.
 Example:
 ```
 enseash % exit
 Bye bye...
 ```
4. **Exit Codes and Signals**: Displays the return code or termination signal of the
last executed command in the prompt.
 Example:
 ```
 enseash % ls
 enseash [exit:0] %
 enseash % kill -9 $$
 enseash [sign:9] %
 ```
5. **Execution Time Measurement**: Displays the execution time of each command in
milliseconds.
 Example:
 ```
 enseash % sleep 1
 enseash [exit:0|1000ms] %
 ```
6. **Complex Commands with Arguments**: Supports commands with multiple arguments.
 Example:
 ```
 enseash % hostname -i
 127.0.0.1
 ```
---
## Upcoming Features
The shell will also include:
7. Input and output redirection using `<` and `>`.
8. Piping commands using `|`.
9. Background process execution using `&`.
---
## How to Use
1. Clone this repository:
 ```
 git clone <repository_url>
 cd enseash
 ```
2. Compile the code:
 ```
 gcc -o enseash enseash.c
 ```
3. Run the shell:
 ```
 ./enseash
 ```
---
## Files in the Repository
- `enseash.c`: The main C program implementing the shell.
- `README.md`: This documentation file.
---
## Contribution
Feel free to fork the repository, raise issues, or contribute via pull requests to
enhance the shell functionality.
---
## License
This project is distributed under the MIT License.
