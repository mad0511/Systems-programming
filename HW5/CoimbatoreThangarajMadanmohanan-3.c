#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <ctype.h>
#include <dirent.h>

#define MAX_FILES 1000

int letterFrequency[26] = {0};
char fileList[MAX_FILES][1000]; // Array to store file paths
int fileCount = 0;

// Function to process files and update letter frequency in a thread
void *processFile(void *arg) {
    int index = *(int *)arg;
    char target = 'a' + index;

    // Loop through each file and update letter frequency for the specific letter
    for (int i = 0; i < fileCount; i++) {
        FILE *fileStream = fopen(fileList[i], "rb");
        char character = 0;
        while ((character = fgetc(fileStream)) != EOF) {
            if (tolower(character) == target) {
                letterFrequency[index]++;
            }
        }
        fclose(fileStream);
    }

    free(arg);
    return NULL;
}

// Function to traverse directories and list files
void processDirectory(char *basePath) {
    printf("Working with directory and files under: %s\n",basePath);
    char path[1000];
    struct dirent *directoryEntry;
    DIR *directory = opendir(basePath);

    // Loop through directory entries
    while ((directoryEntry = readdir(directory)) != NULL) {
        if (strcmp(directoryEntry->d_name, ".") != 0 && strcmp(directoryEntry->d_name, "..") != 0) {
            // Construct full path for the entry
            strcpy(path, basePath);
            strcat(path, "/");
            strcat(path, directoryEntry->d_name);

            if (directoryEntry->d_type == DT_DIR) {
                // Recursive call for directories
                processDirectory(path);
            } else {
                // Add file to the list
                if (fileCount < MAX_FILES) {
                    strcpy(fileList[fileCount], path);
                    fileCount++;
                } else {
                    fprintf(stderr, "Maximum file count reached. Ignoring additional files.\n");
                    break;
                }
            }
        }
    }

    closedir(directory);
}

// Function to create threads and process files
void createThreadsAndProcessFiles() {
    pthread_t threads[26];

    // Create threads for each letter to process files concurrently
    for (int i = 0; i < 26; i++) {
        int *arg = malloc(sizeof(int));
        *arg = i;
        pthread_create(&threads[i], NULL, processFile, arg);
    }

    // Wait for all threads to finish
    for (int i = 0; i < 26; i++) {
        pthread_join(threads[i], NULL);
    }

    // Display letter frequencies after processing files
    for (int i = 0; i < 26; i++) {
        printf("Processing %c with thread id %d\n", ('a' + i), threads[i]);
        printf("Letter frequency for %c: %d\n", ('a' + i), letterFrequency[i]);
    }

}

int main() {
    char buffer[256];
    clock_t start_time = clock();
    char *basePath = getcwd(buffer, 256);

    // Traverse directories and list files
    processDirectory(basePath);
    // Create threads to process files
    createThreadsAndProcessFiles();
    clock_t end_time = clock();

    double total_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    printf("Multi thread execution for each alphabet time: %f seconds\n", total_time);

    return 0;
}
