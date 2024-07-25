#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define MAX_FILES 1000
#define MAX_PATH_LENGTH 1000

int letterFrequency[26] = {0};
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Function to process an individual file and update letter frequency
void processFileAndUpdateFrequency(char *filePath) {
    printf("Processing File: %s\n",filePath);
    FILE *fileStream = fopen(filePath, "rb");

    char character = 0;
    while ((character = fgetc(fileStream)) != EOF) {
        if (isalpha(character)) {
            character = tolower(character);
            if (character >= 'a' && character <= 'z') {
                // Lock the mutex to protect shared data
                pthread_mutex_lock(&mutex);
                letterFrequency[character - 'a']++;
                // Release the mutex
                pthread_mutex_unlock(&mutex);
            }
        }
    }

    fclose(fileStream);
}

// Thread function to process files
void *threadFunction(void *arg) {
    char *filePath = (char *)arg;
    processFileAndUpdateFrequency(filePath);
    return NULL;
}

// Function to explore directories and process files
void processDirectory(char *dirPath) {
    DIR *directory = opendir(dirPath);

    pthread_t threads[MAX_FILES]; // Array to hold thread IDs
    int threadCount = 0;

    struct dirent *dirEntry;
    while ((dirEntry = readdir(directory)) != NULL) {
        char buffer[MAX_PATH_LENGTH] = {0};
        strcat(buffer, dirPath);
        strcat(buffer, "/");
        strcat(buffer, dirEntry->d_name);

        if (dirEntry->d_type == DT_DIR) {
            if (strcmp(dirEntry->d_name, ".") != 0 && strcmp(dirEntry->d_name, "..") != 0) {
                processDirectory(buffer); // Recursive call for directories
            }
        } else if (dirEntry->d_type == DT_REG) {
            // Create a new thread for each file
            pthread_create(&threads[threadCount], NULL, threadFunction, strdup(buffer));
            threadCount++;
        }
    }

    closedir(directory);

    // Wait for all threads to finish
    for (int i = 0; i < threadCount; i++) {
        pthread_join(threads[i], NULL);
    }
}

int main() {
    char currentDir[MAX_PATH_LENGTH];
    char *dirPath = getcwd(currentDir, MAX_PATH_LENGTH);

    clock_t start_time = clock();
    processDirectory(dirPath); // Initiate directory processing

    clock_t end_time = clock();

    // Display letter frequencies after processing files
    for (int i = 0; i < 26; i++) {
        printf("%c: %d\n", ('a' + i), letterFrequency[i]);
    }

    double total_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Multi thread execution for each file time: %f seconds\n", total_time);

    return 0;
}