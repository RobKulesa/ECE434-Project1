#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

void *sorterthread(void *args);
void *reptrhead(void *args);
void merge(int arr[], int l, int m, int r);
void mergeSort(int arr[], int l, int r);

typedef struct sorterarray {
    int* arr;
    int length;
    char* path;
}sorterarray;

typedef struct reparray {
    int num_threads_per_group;
    int group_chunk_length;
    int groupnum;
    int num_groups;
    int group_remainder;
    int* arr;
}reparray;

pthread_mutex_t* mutex;

void *sorterthread(void *args) {
    sorterarray* sorterarr = (sorterarray*) args;
    int* arr = sorterarr->arr;
    int length = sorterarr->length;
    char* path = sorterarr->path;

    FILE* groupfile = fopen(path, "a+");

    mergeSort(arr, 0, length - 1);

    pthread_mutex_lock(mutex);
    for(int i = 0; i < length; ++i) {
        fprintf(groupfile, "%d,", arr[i]);
    }
    pthread_mutex_unlock(mutex);
    fclose(groupfile);
    return (void*) 0;
}

void *repthread(void *args) {
    reparray* reparr = (reparray*) args;
    int num_threads_per_group = reparr->num_threads_per_group;
    int group_chunk_length = reparr->group_chunk_length;
    int groupnum = reparr->groupnum;
    int num_groups = reparr->num_groups;
    int group_remainder = reparr->group_remainder;
    int* arr = reparr->arr;
    pthread_t sorters[num_threads_per_group];
    char path[17];
    sprintf(path, "./tmp/group%d.txt", groupnum+1);
    /* printf("Path: %s\n", path); */

    int thread_chunk_length = group_chunk_length / num_threads_per_group;
    int thread_remainder = group_chunk_length - thread_chunk_length * num_threads_per_group;

    if(groupnum == num_groups - 1) {
        thread_chunk_length = (group_chunk_length + group_remainder) / num_threads_per_group;
        thread_remainder = (group_chunk_length + group_remainder) - thread_chunk_length * num_threads_per_group;
    }
    sorterarray* sorterarrs[num_threads_per_group];
    for(int j = 0; j < num_threads_per_group; j++) {
        int start = j*thread_chunk_length;
        int threadarrlength;
        if(j == num_threads_per_group - 1) {
            threadarrlength = thread_chunk_length + thread_remainder;
        } else {
                threadarrlength = thread_chunk_length;
        }
        sorterarray* sorterarr = (sorterarray*) calloc(1, sizeof(sorterarray));
        sorterarr->arr = (int*) &arr[start];
        sorterarr->length = threadarrlength;
        sorterarr->path = path;
        sorterarrs[j] = sorterarr;
    }

    FILE* groupfile = fopen(path, "w+");
    fclose(groupfile);

    int length = 0;
    for(int i = 0; i < num_threads_per_group; ++i) {
        length += sorterarrs[i]->length;
        pthread_create(&sorters[i], NULL, sorterthread, sorterarrs[i]);
    }
    
    for(int i = 0; i < num_threads_per_group; ++i) {
        pthread_join(sorters[i], NULL);
    }
    groupfile = fopen(path, "r");
    int* totalarr = calloc(length, sizeof(int));
    for(int i = 0; i < length; ++i) {
        fscanf(groupfile, "%d,", &totalarr[i]);
    }
    fclose(groupfile);
    mergeSort(totalarr, 0, length - 1);
    if(length <= 100) {
        printf("Group Sorted Arr: ");
        for(int i = 0; i < length; ++i) {
            printf("%d ", totalarr[i]);
        }
        printf("\n");
    }
    return (void*) 0;
}

int main(int argc, char** argv) {
    srand(getpid());
    mutex = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t));
    int list_length;
    printf("Enter length of integer list (O(1k) - O(1m)): ");
    scanf("%d", &list_length);

    int numArray[list_length];
    for(int i = 0; i < list_length; ++i) {
        numArray[i] = rand() % 10000000;
    }

    /* printf("Arr is: ");
    for(int i = 0; i < list_length; ++i) {
        printf("%d ", numArray[i]);
    }
    printf("\n"); */

    int num_groups;
    printf("Enter number of thread groups (rec. 4): ");
    scanf("%d", &num_groups);

    int num_threads_per_group;
    printf("Enter number of threads per group: (try 4, 100, 1000): ");
    scanf("%d", &num_threads_per_group);


    pthread_t groups[num_groups];
    reparray* reparrs[num_groups];
    clock_t groupTimers[num_groups];

    int group_chunk_length = list_length / num_groups;
    int group_remainder = list_length - group_chunk_length * num_groups;
    for(int i = 0; i < num_groups; ++i) {
        int start = i*group_chunk_length;
        reparray* reparr = (reparray*) calloc(1, sizeof(reparray)); 
        
        
        reparr->groupnum = i;
        reparr->num_groups = num_groups;
        reparr->group_remainder = group_remainder;
        reparr->group_chunk_length = group_chunk_length;
        reparr->arr = (int*) calloc(group_chunk_length, sizeof(int));
        reparr->arr = &numArray[start];
        reparr->num_threads_per_group = num_threads_per_group;
        reparrs[i] = reparr;
        groupTimers[i] = clock();
        pthread_create(&groups[i], NULL, repthread, reparrs[i]);
    }

    for(int i = 0; i < num_groups; ++i) {
        pthread_join(groups[i], NULL);
    }

    for(int i = 0; i < num_groups; ++i) {
        for(int j = 0; j < num_threads_per_group; j++) {
            if(j==num_threads_per_group-1) { 
                groupTimers[i] = clock() - groupTimers[i]; 
                printf("Group %d took %.3f ms!\n", i+1, ((double) groupTimers[i])/CLOCKS_PER_SEC*100);
            }
        }
        free(reparrs[i]);
    }
}

void merge(int arr[], int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;
 
    int L[n1], R[n2];
 
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];
 
    i = 0; 
    j = 0; 
    k = l; 

    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(int arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;

        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
 
        merge(arr, l, m, r);
    }
}