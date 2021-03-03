#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void writeInFile(int fd, const char *buff, int len);
void copyFile(int fd, const char *file_in);

int main(int argc, char** argv) {
    if(argc < 4) {
        fprintf(stderr, "ERROR: You need at least 3 file names\n");
        exit(EXIT_FAILURE);
    }
    FILE** files = malloc(sizeof(FILE)*(argc-2));
    
    int i;
    for(i = 1; i < argc - 1; ++i) {
        files[i] = fopen(argv[i], "r");
        if(files[i] == NULL) {
            fprintf(stderr, "ERROR: Failed to open file %s\n", argv[i]);
            exit(EXIT_FAILURE);
        }
    }
    
    char* file_in = argv[argc-1];
    fclose(fopen(file_in, "w"));
    for(i = 1; i < argc - 1; ++i) {
        FILE* curr = files[i];
        copyFile(fileno(curr), file_in);
    }

    for(i = 1; i < argc - 1; ++i) {
        fclose(files[i]);
    }
    free(files);

    return EXIT_SUCCESS;
}

void writeInFile(int fd, const char *buff, int len) {
    if(write(fd, buff, len) < 0) {
        printf("Failed to write to file.\n");
        exit(EXIT_FAILURE);
    }    
    return;
}

void copyFile(int fd, const char *file_in) {
    int fileSize = (int)lseek(fd, (size_t)0, SEEK_END);
    lseek(fd, (off_t)0, SEEK_SET);
    char buffer[fileSize];
    buffer[fileSize] = '\0';
    read(fd, (void *)buffer, (size_t)fileSize);
    FILE* fileToWrite = fopen(file_in, "a");
    writeInFile(fileno(fileToWrite), buffer, fileSize);
    return;
}