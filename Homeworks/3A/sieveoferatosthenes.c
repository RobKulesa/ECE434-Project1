#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

void *sieveoferatosthenes(void *n);

void *sieveoferatosthenes(void *n) {
    int num = *((int*) n);
    int prime[num + 1];
    memset(prime, 1, sizeof(prime));
 
    for(int p = 2; p * p <= num; p++) {
        if (prime[p]) {
            for (int i = p * p; i <= num; i += p)
                prime[i] = 0;
        }
    }
 
    for (int p = 2; p <= num; p++) {
        if(prime[p]) { 
            printf("%d ", p);
        }
    }
    printf("\n");
}
 

int main(int argc, char** argv) {
    int n;
    pthread_t t_id;
    while(1) {
        printf("Enter an integer: ");
        scanf("%d", &n);
        pthread_create(&t_id, NULL, sieveoferatosthenes, (void*) &n);
        pthread_join(t_id, NULL);
    }
    return 0;
}