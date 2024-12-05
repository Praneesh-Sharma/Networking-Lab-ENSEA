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
jeudi 5 décembre 2024, 10:29:40 (UTC+0100)
enseash [exit:0|2ms] % enseash % 
 ```
3. **Exit Management**: Handles the `exit` command or `<Ctrl>+D` to terminate the shell
gracefully.
 Example:
 ```
enseash [exit:0|2ms] % enseash % exit
Bye bye...
 ```
4. **Exit Codes and Signals**: Displays the return code or termination signal of the
last executed command in the prompt.
 Example:
 ```
 enseash [exit:0|1ms] % enseash % ls
 main  main.c  README.md
 ```
5. **Execution Time Measurement**: Displays the execution time of each command in
milliseconds.
 Example:
 ```
 enseash % sleep 1
enseash [exit:0|1005ms] % enseash % 
 ```
6. **Complex Commands with Arguments**: Supports commands with multiple arguments.
 Example:
 ```
 enseash [exit:0|1005ms] % enseash % hostname -i
127.0.1.1
 ```
---
## How to Use
1. Clone this repository:
 ```
 git clone <https://github.com/Praneesh-Sharma/Networking-Lab-ENSEA.git>
 cd Networking-Lab-ENSEA
 ```
2. Compile the code:
 ```
 gcc -o main main.c
 ```
3. Run the shell:
 ```
 ./main
 ```
---
## Files in the Repository
- `main.c`: The main C program implementing the shell.
- `README.md`: This documentation file.
---
## Contribution
This project was made by Praneesh Sharma and Shibaa Naik of ENSEA International Batch
