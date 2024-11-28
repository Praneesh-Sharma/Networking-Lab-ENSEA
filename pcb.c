#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
void print_PCB()
{
// TODO: complete this function
pid_t pid = getpid(); //Get the process ID
pid_t ppid= getppid(); //Get the parent process ID
uid_t uid = getuid(); //Get the user ID
gid_t gid = getgid(); //Get the Group ID

printf("PCB | PPID: %d\n", ppid);
printf("PCB | PID: %d\n", pid);
printf("PCB | UID: %d\n", uid);
printf("PCB | GID: %d\n", gid); 
}
int main()
{
pid_t ret = fork();
printf("\nfork() returned: %d\n", ret);
print_PCB();
exit(EXIT_SUCCESS);
}