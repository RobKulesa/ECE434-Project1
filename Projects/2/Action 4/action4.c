#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

void sig_handler(int signum);

typedef struct threadFuncArgs {
    struct sigaction* threadfunc_sigaction;
    struct sigaction* ignore_sigaction;
}threadFuncArgs;

pthread_mutex_t* mutex;

void sig_handler(int signum) {
    pthread_mutex_lock(mutex);
    printf("Thread %ld received signum: %d\n", pthread_self(), signum);
    pthread_mutex_unlock(mutex);
}


void* threadFunc(void* args) {
    struct sigaction* threadfunc_sigaction = (struct sigaction*) malloc(sizeof(struct sigaction));
    struct sigaction* ignore_sigaction = (struct sigaction*) malloc(sizeof(struct sigaction));

    threadfunc_sigaction->sa_handler = &sig_handler;
    ignore_sigaction->sa_handler = SIG_IGN;


    sigfillset(&(threadfunc_sigaction->sa_mask));
    sigfillset(&(ignore_sigaction->sa_mask));
    sigaction(SIGINT, threadfunc_sigaction, NULL);
    sigaction(SIGABRT, threadfunc_sigaction, NULL);
    sigaction(SIGFPE, threadfunc_sigaction, NULL);

    sigaction(SIGILL, ignore_sigaction, NULL);
    sigaction(SIGCHLD, ignore_sigaction, NULL);
    sigaction(SIGSEGV, ignore_sigaction, NULL);
    sigaction(SIGHUP, ignore_sigaction, NULL);
    sigaction(SIGTSTP, ignore_sigaction, NULL);

    while(1) {
        pthread_mutex_lock(mutex);
        printf("I am thread %ld waiting for signal...\n", pthread_self());
        pthread_mutex_unlock(mutex);
        sleep(5);
    }

    return (void*) 0;
}

int main(int argc, char** argv) {
    mutex = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t));
    struct sigaction* main_sigaction = (struct sigaction*) malloc(sizeof(struct sigaction));
    main_sigaction->sa_handler = SIG_IGN;
    struct sigaction* old_int = (struct sigaction*) malloc(sizeof(struct sigaction));
    struct sigaction* old_abrt = (struct sigaction*) malloc(sizeof(struct sigaction));
    struct sigaction* old_fpe = (struct sigaction*) malloc(sizeof(struct sigaction));
    struct sigaction* old_ill = (struct sigaction*) malloc(sizeof(struct sigaction));
    struct sigaction* old_chld = (struct sigaction*) malloc(sizeof(struct sigaction));
    struct sigaction* old_segv = (struct sigaction*) malloc(sizeof(struct sigaction));
    struct sigaction* old_hup = (struct sigaction*) malloc(sizeof(struct sigaction));
    struct sigaction* old_tstp = (struct sigaction*) malloc(sizeof(struct sigaction));

    sigfillset(&(main_sigaction->sa_mask));
    sigaction(SIGINT, main_sigaction, old_int);
    sigaction(SIGABRT, main_sigaction, old_abrt);
    sigaction(SIGFPE, main_sigaction, old_fpe);
    sigaction(SIGILL, main_sigaction, old_ill);
    sigaction(SIGCHLD, main_sigaction, old_chld);
    sigaction(SIGSEGV, main_sigaction, old_segv);
    sigaction(SIGHUP, main_sigaction, old_hup);
    sigaction(SIGTSTP, main_sigaction, old_tstp);

    int num_groups = 4;

    pthread_t groups[num_groups];
    
 


    for(int i = 0; i < num_groups; ++i) {
        pthread_create(&groups[i], NULL, threadFunc, NULL);
        sleep(1);
    }

    //ACTION 4 Q2: call pthread_kill before this point

    for(int i = 0; i < num_groups; ++i) { 
        pthread_join(groups[i], NULL);
    }

    sigaction(SIGINT, old_int, NULL);
    sigaction(SIGABRT, old_abrt, NULL);
    sigaction(SIGFPE, old_fpe, NULL);
    sigaction(SIGILL, old_ill, NULL);
    sigaction(SIGCHLD, old_chld, NULL);
    sigaction(SIGSEGV, old_segv, NULL);
    sigaction(SIGHUP, old_hup, NULL);
    sigaction(SIGTSTP, old_tstp, NULL);

    free(old_int);
    free(old_abrt);
    free(old_fpe);
    free(old_ill);
    free(old_chld);
    free(old_segv);
    free(old_hup);
    free(old_tstp);
    free(main_sigaction);
    free(mutex);
}

