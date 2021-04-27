#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

pthread_t    threadid1; 
pthread_t    threadid2; 
pthread_t    threadid3; 

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


void* threadFunc1(void* args) {
    struct sigaction* threadfunc_sigaction = (struct sigaction*) malloc(sizeof(struct sigaction));
    struct sigaction* ignore_sigaction = (struct sigaction*) malloc(sizeof(struct sigaction));

    threadfunc_sigaction->sa_handler = &sig_handler;
    ignore_sigaction->sa_handler = SIG_IGN;


    sigfillset(&(threadfunc_sigaction->sa_mask));
    sigfillset(&(ignore_sigaction->sa_mask));
    signal(SIGINT, sig_handler);
    signal(SIGABRT, sig_handler);
    signal(SIGFPE, sig_handler);

    sigaction(SIGILL, ignore_sigaction, NULL);
    sigaction(SIGCHLD, ignore_sigaction, NULL);
    sigaction(SIGSEGV, ignore_sigaction, NULL);
    sigaction(SIGHUP, ignore_sigaction, NULL);
    sigaction(SIGTSTP, ignore_sigaction, NULL);
    threadid1= pthread_self();

    return (void*) 0;
}

void* threadFunc2(void* args) {
    struct sigaction* threadfunc_sigaction = (struct sigaction*) malloc(sizeof(struct sigaction));
    struct sigaction* ignore_sigaction = (struct sigaction*) malloc(sizeof(struct sigaction));

    threadfunc_sigaction->sa_handler = &sig_handler;
    ignore_sigaction->sa_handler = SIG_IGN;


    sigfillset(&(threadfunc_sigaction->sa_mask));
    sigfillset(&(ignore_sigaction->sa_mask));
    signal(SIGINT, sig_handler);
    signal(SIGABRT, sig_handler);
    signal(SIGFPE, sig_handler);

    sigaction(SIGILL, ignore_sigaction, NULL);
    sigaction(SIGCHLD, ignore_sigaction, NULL);
    sigaction(SIGSEGV, ignore_sigaction, NULL);
    sigaction(SIGHUP, ignore_sigaction, NULL);
    sigaction(SIGTSTP, ignore_sigaction, NULL);

    threadid2=pthread_self();
   
    return (void*) 0;
}

void* threadFunc3(void* args) {
    struct sigaction* threadfunc_sigaction = (struct sigaction*) malloc(sizeof(struct sigaction));
    struct sigaction* ignore_sigaction = (struct sigaction*) malloc(sizeof(struct sigaction));

    threadfunc_sigaction->sa_handler = &sig_handler;
    ignore_sigaction->sa_handler = SIG_IGN;


    sigfillset(&(threadfunc_sigaction->sa_mask));
    sigfillset(&(ignore_sigaction->sa_mask));
    signal(SIGINT, sig_handler);
    signal(SIGABRT, sig_handler);
    signal(SIGFPE, sig_handler);

    sigaction(SIGILL, ignore_sigaction, NULL);
    sigaction(SIGCHLD, ignore_sigaction, NULL);
    sigaction(SIGSEGV, ignore_sigaction, NULL);
    sigaction(SIGHUP, ignore_sigaction, NULL);
    sigaction(SIGTSTP, ignore_sigaction, NULL);

    threadid3=pthread_self();
   

    return (void*) 0;
}

void* threadFunc4(void* args) {

    sleep(4);
    void *a1,*a2, *a3; 
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
    pthread_kill(threadid1, SIGINT);
    sleep(1);
    pthread_kill(threadid1, SIGINT);
    
    pthread_kill(threadid2, SIGINT);
    sleep(1);
    pthread_kill(threadid2, SIGINT);
    
    pthread_kill(threadid3, SIGINT);
    sleep(1);
    pthread_kill(threadid3, SIGINT);
    
    pthread_kill(pthread_self(), SIGINT);
    sleep(1);
    pthread_kill(pthread_self(), SIGINT);
    
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
    signal(SIGINT, sig_handler);
    signal(SIGABRT, sig_handler);
    signal(SIGFPE, sig_handler);
    sigaction(SIGILL, main_sigaction, old_ill);
    sigaction(SIGCHLD, main_sigaction, old_chld);
    sigaction(SIGSEGV, main_sigaction, old_segv);
    sigaction(SIGHUP, main_sigaction, old_hup);
    sigaction(SIGTSTP, main_sigaction, old_tstp);


    int num_groups = 4;

    pthread_t groups[num_groups];
    

    pthread_create(&groups[0], NULL, threadFunc1, NULL);
    pthread_create(&groups[1], NULL, threadFunc2, NULL);
    pthread_create(&groups[2], NULL, threadFunc3, NULL);
    pthread_create(&groups[3], NULL, threadFunc4, NULL);
       
    sleep(1);
    for(int i = 0; i < num_groups; ++i) { 
        void *returnValue;
        pthread_join(groups[i], &returnValue);
        printf("Status: %d\n",  (int)returnValue);

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
