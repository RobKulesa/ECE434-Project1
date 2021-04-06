#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

void *sortsubarray(void *arr);
void merge(int arr[], int l, int m, int r);
void mergeSort(int arr[], int l, int r);

typedef struct subarray {
    int* arr;
    int length;
}subarray;

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
void *sortsubarray(void *arr) {
    subarray* threadarray = (subarray*) arr;
    int* numArray = threadarray->arr;
    int length = threadarray->length;

    /* for(int i = 0; i < length; ++i) {
        printf("Thread %d: %d\n", (int) pthread_self(), numArray[i]);
    } */

    printf("Thread %d Before Sorted: ", (int) pthread_self());
    for(int i = 0; i < length; ++i) {
        printf("%d ", numArray[i]);
    }
    printf("\n");

    mergeSort(numArray, 0, length - 1);

    printf("Thread %d After Sorted: ", (int) pthread_self());
    for(int i = 0; i < length; ++i) {
        printf("%d ", numArray[i]);
    }
    printf("\n");

    return (void*) 0;
}
 

int main(int argc, char** argv) {
    int list_length;
    printf("Enter length of integer list (1k - 1m): ");
    scanf("%d", &list_length);

    int numArray[list_length];
    for(int i = 0; i < list_length; ++i) {
        numArray[i] = rand() % list_length;
    }

    int num_groups;
    printf("Enter number of thread groups: ");
    scanf("%d", &num_groups);

    int num_threads_per_group;
    printf("Enter number of threads per group: ");
    scanf("%d", &num_threads_per_group);

    printf("Arr is: ");
    for(int i = 0; i < list_length; ++i) {
        printf("%d ", numArray[i]);
    }
    printf("\n");


    pthread_t groups[num_groups][num_threads_per_group];
    subarray* threadarrs[num_groups][num_threads_per_group];
    
    int group_chunk_length = list_length / num_groups;
    int group_remainder = list_length - group_chunk_length * num_groups;
    for(int i = 0; i < num_groups; ++i) {
        int thread_chunk_length = group_chunk_length / num_threads_per_group;
        int thread_remainder = group_chunk_length - thread_chunk_length * num_threads_per_group;

        if(i == num_groups - 1) {
            thread_chunk_length = (group_chunk_length + group_remainder) / num_threads_per_group;
            thread_remainder = (group_chunk_length + group_remainder) - thread_chunk_length * num_threads_per_group;
        }

        for(int j = 0; j < num_threads_per_group; j++) {
            int start = i*group_chunk_length + j*thread_chunk_length;
            int length = thread_chunk_length;
            subarray *threadarr = (subarray*) calloc(1, sizeof(subarray*));
            threadarrs[i][j]=threadarr;
            threadarr->arr = (int*) &numArray[start];
            if(j == num_threads_per_group - 1) {
                threadarr->length = length + thread_remainder;
            } else {
                threadarr->length = length;
            }

            pthread_create(&groups[i][j], NULL, sortsubarray, threadarr);
            //pthread_join(groups[i][j], NULL);
        }
    }

    for(int i = 0; i < num_groups; ++i) {
        for(int j = 0; j < num_threads_per_group; j++) {
            pthread_join(groups[i][j], NULL);
            subarray *threadarr = threadarrs[i][j];
            free(threadarr);
        }
    }
}

