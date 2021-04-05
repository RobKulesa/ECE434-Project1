#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void handler(int signum) {
    //gn(0,-3);
}

int main(void) {
    int i, p[2], writefd[4];
    pid_t pid[4];
    signal(SIGUSR1, handler);
    for(i = 0; i < 4; i++) {
        pipe(p);
        pid[i] = fork();
        if(pid[i] == 0) {
            printf("\tYo PID %d trying to read!\n", getpid());
            close(p[1]);
            read(p[0], &pid[i], sizeof(pid_t));
            printf("Process ID %d trying to kill: %d\n", getpid(), pid[i]);
            if(pid[i]>0) kill(pid[i], SIGKILL);
            printf("Process ID %d tried to kill: %d\n", getpid(), pid[i]);
        }
        //gn1(i, pid[i]);
        close(p[0]);
        writefd[i] = p[1];
    }
    write(writefd[1], &pid[0], sizeof(pid_t));
    close(writefd[3]);
    wait(NULL);
    write(writefd[2], &pid[1], sizeof(pid_t));
    //gn1(-1,-2);
    return 0;
}
