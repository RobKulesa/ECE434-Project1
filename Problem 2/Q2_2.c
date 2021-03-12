#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <pthread.h>

// Structure definition
struct dataStruct {
    int largestIntFound;
    int hiddenKeysFound;
    long long int sum;
} myData;


// Helper definition
void selfIdentify (FILE* outputFile) {
	fprintf(outputFile, "Hi I'm process %d and my parent is %d\n", getpid(), getppid());
}

int considerAndFork (
        pid_t* children, 
        int *numChildren, 
        int maxChildren, 
        struct dataStruct** pipes, 
        struct dataStruct* *pipeToParent
    ) {
    // TODO change tje pipes and pipe to parent 
    if (*numChildren == maxChildren) {
        return -1;
    } else {
        pid_t temp = fork();
        if (temp == -1) {
            return 0;
        } else if (temp == 0) {
            // Child
            *pipeToParent = pipes[*numChildren];
            *numChildren = 0;
            for (int i = 0; i<maxChildren; i++){
                children[i] = 0; 
            }
            return 2;
        } else {
            // Parent
            children[*numChildren] = temp;
            if (*++numChildren < maxChildren) {
                pipe(pipes[*numChildren]);
            }
            return 1;
        }
        
    }
}

FILE* generateFile(char* fileLocation, int fileIntsCount, int maxHiddenKeys){
    // Initialize file
    FILE* inputFile = fopen(fileLocation, "w+"); 
    if(inputFile == NULL) { 
        printf("ERROR: Could not open file %s\n", fileLocation); 
        return NULL; 
    }
    int keyCount = 0;

    // Populate file
    for(int i = 1; i <= fileIntsCount; i++) {
        if(i == fileIntsCount) {
            fprintf(inputFile, "%d", rand() % 100);
            break;
        }
        if((keyCount < maxHiddenKeys) && (i >= 500) && (i % 75 == 0)) {
            fprintf(inputFile, "%d\n", -10);
            keyCount++;
            continue; 
        }
        fprintf(inputFile, "%d\n", rand() % 100);
    }
    fclose(inputFile);

    
    return inputFile;
}

int main(int argc, char** argv) {
    // Initiating phase:

    // Constants (arguments)
    char* fileLocation = argv[1];           // Location of the input file
    int count = atoi(argv[2]);              // Count of number of integers to put in input file
    int maxHiddenKeyCount = atoi(argv[3]);  // Count og number of hidden keys to put in input file
    int maxProcessCount = atoi(argv[4]);    // Maximum number of processes

    // Constants (Magic number)
    int maxChildrenCount = 3;   // This is a randomly chosen X - can be changed here to an 
                                // integer greater than 0 (including 1), and the code will work

    // Files:
    
    // Input:
    // Generating file:
    FILE* inputFile = generateFile(argv[1], count, maxHiddenKeyCount);
    if (inputFile == NULL) {
        return 0;
    }

    // Scanning input file to an array:
    inputFile = fopen(fileLocation, "r");
    int numbers[count], i;
    for(i = 0; i < count; i++) {
        fscanf(inputFile, "%d", &numbers[i]);
    }
    fclose(inputFile);

    // Output:
    // Initialize file
    FILE* outputFile = fopen("output.txt", "w");
    if(outputFile == NULL) { 
        printf("ERROR: Could not open file %s\n", argv[1]); 
        return 0; 
    }
    
    // Selected edge cases:
    if (count < 3) {
        maxProcessCount = 1;
    } else if (count-1 <= maxProcessCount) {
        maxProcessCount = count-2;
    }
    

    // Forking phase:

    // Local Variables:
    pid_t children [maxChildrenCount];
    int numChildren = 0;

    int arrayRangeMin = 0;
    int arrayRangeMax = count;

    // Shared memory variables:
    //   number of processes running
    //   largest integer
    //   numHiddenFound
    //   sum

    // Shared memory
    // TODO figure out the mutex shared memory and implement
    int totalProcesses = 1;
    pthread_mutex_t totalProcessesMutex = PTHREAD_MUTEX_INITIALIZER;

    
    // List of stuff for convenience
    // Constants:
    //   Length of input file
    //   number of hidden keys
    //   number of processes
    //   max number of children

    // Local variables:
    //   children array
    //   number of children
    //   input file allocated area bottom limit
    //   input file allocated area top limit

    // Shared memory using mutex:
    //   number of processes running
    //   largest integer
    //   sum
    //   numHiddenFound

    

    

    int temp = 0;

    // Forking loop:
    while ((*totalProcessesPointer < maxProcessCount) && (numChildren < maxChildrenCount)){
        temp = considerAndFork(*children, &numChildren, maxChildrenCount, pipes, &pipeToParent);
        if (temp == -1) {
            // fork() error
            printf("ERROR: Could not fork %s\n", argv[1]); 
            return 0;
        } else if (temp == 0) {
            // max children reached
            break;
        } else if (temp == 1) {
            // Parent
            *totalProcessesPointer++;
            arrayRangeMax = (arrayRangeMax+arrayRangeMin)/2;
        } else {
            // Child
            *totalProcessesPointer++;
            arrayRangeMin = (arrayRangeMax+arrayRangeMin)/2;
            selfIdentify(outputFile);
        }
    }


    // Running phase:
    
    // Variables:
    int largestIntFound = 0;
    long long int sum = 0;
    int hiddenKeysFound = 0;

    // Array analysis loop:
    for(i = arrayRangeMin; i < arrayRangeMax; ++i) {
        sum += numbers[i];
        if(numbers[i] > largestIntFound) largestIntFound = numbers[i];
        if(numbers[i] == -10) {
            hiddenKeysFound++;
            fprintf(outputFile, "Hi I am process %d and I found the hidden key in position numbers[%d]\n", getpid(), i);
        }
    }
    
    // Closing forks phase:

    // TODO when close: if  do shmdt() 

    // Ensuring synchronicity
    sleep(1); 

    // Piping to parent
    struct dataStruct myData = {
        largestIntFound,
        hiddenKeysFound,
        sum
    };

    myData.largestIntFound = largestIntFound;
    myData.hiddenKeysFound = hiddenKeysFound;
    myData.sum = sum;

    ssize_t bytesWritten = 0;

    do {
        bytesWritten = write(pipeToParent[1], myData, sizeof(myData));
    } while (bytesWritten != sizeof(myData))

    pipeToParent[1] = myData;

    // Collecting Pipe from children
    for (int i = 0; i < numChildren; i++){
        struct dataStruct childData = pipes[i][0];
        if (largestIntFound > ) {

        }
    }

    fprintf(outputFile, "Max = %d, Avg = %.2f\n", largestIntFound, average);
    fclose(outputFile);
    return 0; 
}