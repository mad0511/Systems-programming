#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define MAX_FILES 1000
#define MAX_PATH_LENGTH 256

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int frequency[26] = {0};

// Function to process an individual file and update letter frequency
void processFile(char *filePath) {
    printf("Processing File: %s\n",filePath);
    FILE *fileStream = fopen(filePath, "rb");

    char character = 0;
    while ((character = fgetc(fileStream)) != EOF) {
        if (isalpha(character)) {
            character = tolower(character);
            if (character >= 'a' && character <= 'z') {
                // Lock the mutex to protect shared data
                pthread_mutex_lock(&mutex);
                frequency[character - 'a']++;
                // Release the mutex
                pthread_mutex_unlock(&mutex);
            }
        }
    }

    fclose(fileStream);
}

// Function to traverse directories and process files
void *processDirectory(void *arg) {
    char *dir_path = (char *)arg;
    DIR *dir = opendir(dir_path);
    printf("Working with directory: %s\n",dir_path);

    struct dirent *dir_entry;
    while ((dir_entry = readdir(dir)) != NULL) {
        char buffer[MAX_PATH_LENGTH] = {0};
        strcat(buffer, dir_path);
        strcat(buffer, "/");
        strcat(buffer, dir_entry->d_name);
        
        if (dir_entry->d_type == DT_DIR) {
            if (strcmp(dir_entry->d_name, ".") != 0 && strcmp(dir_entry->d_name, "..") != 0) {
                // Create a new thread for each subdirectory
                pthread_t tid;
                pthread_create(&tid, NULL, processDirectory, strdup(buffer));
                pthread_join(tid, NULL);
            }
        } else if (dir_entry->d_type == DT_REG) {
            // Create a new thread for each file
            pthread_t tid;
            pthread_create(&tid, NULL, (void *(*)(void *))processFile, strdup(buffer));
            pthread_join(tid, NULL);
        }
    }

    closedir(dir);
    free(dir_path);
    return NULL;
}



int main() {
    char current_dir[MAX_PATH_LENGTH];
    char *dir_path = getcwd(current_dir, MAX_PATH_LENGTH);
    
    clock_t start_time = clock();

    // Create a thread for the root directory
    pthread_t main_tid;
    pthread_create(&main_tid, NULL, processDirectory, strdup(dir_path));
    pthread_join(main_tid, NULL);

    clock_t end_time = clock();

    // Display letter frequencies after processing files
    for (int i = 0; i < 26; i++) {
        printf("%c: %d\n", ('a' + i), frequency[i]);
    }

    double total_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Multi thread execution for each file and directory time: %f seconds\n", total_time);

    // Destroy the mutex
    pthread_mutex_destroy(&mutex);
    return 0;
}