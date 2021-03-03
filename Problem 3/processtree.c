#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>

/**
 * Answers to additional questions:
 * 
 * 1. What happens if root process A is terminated prematurely, by issuing: kill -KILL <pid>?
 * Ans: Process A is terminated, but none of the other processes are terminated. They will be adopted by init as they are orphan processes.
 * 
 * 2. What happens if you display the process tree with root getpid() instead of pid() in main()? Which other processes appear in the tree and why?
 * Ans: pid() is not a C function.
 * 
 * 3. What is the maximum random tree you can generate with your program? Why?
 * Ans: The only limitation is storage, as the OS can pick and choose different proccesses to run/pause at different times.
 * 
*/
int main(int argc, char** argv) {
    int status;
    printf("I am process %d, with parent %d\n", getpid(), getppid());
    pid_t pid_arr[2];
    pid_arr[0] = fork();
    if(pid_arr[0] == 0) {
        printf("\t|--I am process %d, with parent %d\n", getpid(), getppid());
        printf("\t|--I am process %d, I finished!\n", getpid());
    } else {
        pid_arr[1] = fork();
        if(pid_arr[1] == 0) {
            printf("\t|--I am process %d, with parent %d\n", getpid(), getppid());
            pid_t pid = fork();
            if(pid == 0) {
                printf("\t\t|--I am process %d, with parent %d\n", getpid(), getppid());
                printf("\t\t|--I am process %d, I finished!\n", getpid());
            } else {
                printf("\t|--I am process %d, I'm waiting for my children to finish\n", getpid());
                waitpid(pid, NULL);
                printf("\t|--I am process %d, I finished waiting\n", getpid());
                printf("\t|--I am process %d, I finished!\n", getpid());
            }
        } else {
            printf("I am process %d, I'm waiting for my children to finish\n", getpid());
            waitpid(pid_arr[0], NULL);
            waitpid(pid_arr[1], NULL);
            printf("I am process %d, I finished waiting\n", getpid());
            printf("I am process %d, I finished!\n", getpid());
        }
    }
    return 0;
}

