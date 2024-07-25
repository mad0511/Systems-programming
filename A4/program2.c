#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h> // Include the sys/stat.h header file

int child_count = 0;

// Function to print the contents of the directory and fork child processes for subdirectories
void print_directory_contents(char *path) {
    DIR *dir = opendir(path);
    if (dir == NULL) {
        perror("Unable to open directory");
        exit(EXIT_FAILURE);
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        // Skip the current and parent directory entries
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        // Get the current working directory
        char current_path[1024];
        getcwd(current_path, sizeof(current_path));

        // Print the current process ID, parent process ID, current directory, and file name
        printf("PID %d (Parent PID %d): %s/%s\n", getpid(), getppid(), current_path, entry->d_name);

        struct stat statbuf; // Ensure sys/stat.h is included
        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);
        // If the entry is a directory, fork a child process
        if (stat(full_path, &statbuf) == 0 && S_ISDIR(statbuf.st_mode)) {
            // Print forking information
            printf("Forking at %s/%s\n", current_path, entry->d_name);
            pid_t pid = fork();
            if (pid < 0) {
                perror("Fork failed");
                exit(EXIT_FAILURE);
            } else if (pid == 0) {
                // Recursively call the function for the subdirectory
                print_directory_contents(full_path);
                // Print the exit message for the child process
                printf("PID %d: Exiting from %s\n", getpid(), current_path);
                exit(0);
            } else {
                // Increment the child count and wait for child processes to finish
                child_count++;
                wait(NULL);
            }
        }
    }
    closedir(dir);
}

// Main function to call the print_directory_contents function and wait for child processes to finish
int main(int argc, char *argv[]) {
    // Check if the command line argument for the directory is provided
    if (argc < 2) {
        fprintf(stderr, "Usage: %s [directory]\n", argv[0]);
        return 1;
    }

    // Store the provided directory
    char *directory = argv[1];
    // Call the function to print directory contents and handle child processes
    print_directory_contents(directory);

    // Wait for all child processes to finish before exiting
    while (child_count > 0) {
        wait(NULL);
        child_count--;
    }

    // Get the current working directory and print the exit message for the main process
    char current_path[1024];
    getcwd(current_path, sizeof(current_path));
    printf("PID %d: Exiting from %s\n", getpid(), current_path);
    return 0;
}
