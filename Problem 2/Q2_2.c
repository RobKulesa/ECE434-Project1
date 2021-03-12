#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

void selfIdentify(FILE* outputFile){
	fprintf(outputFile, "Hi I'm process %d and my parent is %d\n", getpid(), getppid());
}

void printFoundHiddenKey(FILE* outputFile, int hiddenKeyIndex){
	fprintf(outputFile, "Hi I am process %d and I found the hidden key in position A[%d].\n", getpid(), hiddenKeyIndex);
}

void ConsiderAndFork(pid_t* children, int numChildren, int maxChildren){
    if (numChildren == maxChildren){
        return;
    } else {
        pid_t temp = fork();
        if (temp == 0){
            // Child
            numChildren = 0;
            for (int i = 0; i<maxChildren; i++){
                children[i] = 0; 
            }
        } else {
            // Parent
            children[numChildren] = temp;
            numChildren++;
        }
        
    }
}


int main(int argc, char** argv) {
    // Initiating:

    // Constants (arguments)
    int count = atoi(argv[2]);
    int maxHiddenKeyCount = atoi(argv[3]);
    int maxProcessCount = atoi(argv[4]);
    int maxChildrenCount = 3;   // This is a randomly chosen X - can be changed here to an 
                                // integer greater than 0 (including 1), and the code will work

    // Files (input and output)
    FILE* fp; 
    FILE* outputFile = fopen("output.txt", "w");
    if(outputFile == NULL) { 
        printf("ERROR: Could not open file %s\n", argv[1]); 
        return 0; 
    }

    // Forking phase:

    // Variables
    int totalProcesses = 1;
    int* totalProcessesPointer = &totalProcesses;
    pid_t children []

    while (*totalProcessesPointer < maxProcessCount){
        makeFork()
    }

	selfIdentify(outputFile);

    
    fp = fopen(argv[1], "w+"); 
    if(fp == NULL) { 
        printf("ERROR: Could not open file %s\n", argv[1]); 
        return 0; 
    }

    int maxNumber = 0;
    int keyCount = 0;
//My addition
    while(maxProcessCount>maxHiddenkeyCount){
        ConsiderAndFork();
        keyCount++;
    }

// 
    for(int i = 1; i <= count; ++i) {
        if(i == count) {
            fprintf(fp, "%d", rand() % 100);
            break;
        }
        if(keyCount < maxHiddenKeyCount && i >= 500 && i % 75 == 0) {
            fprintf(fp, "%d\n", -10);
            ++keyCount;
            continue; 
        }
        fprintf(fp, "%d\n", rand() % 100);
    }
    fclose(fp);
    fp = fopen(argv[1], "r");
    int numbers[count], i;
    for(i = 0; i < count; ++i) {
        fscanf(fp, "%d", &numbers[i]);
    }

    int max = 0;
    long long int sum = 0;
    for(i = 0; i < count; ++i) {
        sum += numbers[i];
        if(numbers[i] > max) max = numbers[i];
        if(numbers[i] == -10) {
            fprintf(outputFile, "Hi I am process %d and I found the hidden key in position numbers[%d]\n", getpid(), i);
        }
    }
    fprintf(outputFile, "Max = %d, Avg = %.2f\n", max, (double) sum / (double) count);


    fclose(outputFile);
    return 0; 
}