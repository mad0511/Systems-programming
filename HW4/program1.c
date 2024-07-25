#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

// Function to search the directory based on the provided criteria
void search_directory(const char *path, const char *filename, const char *uid, const char *type, int recursive) {
    DIR *dir;
    struct dirent *entry;
    struct stat filestat;

    // Open the directory
    dir = opendir(path);
    if (!dir) {
        fprintf(stderr, "Error opening directory %s\n", path);
        exit(EXIT_FAILURE);
    }

    // Iterate through the directory entries
    while ((entry = readdir(dir)) != NULL) {
        char fullpath[1024];
        snprintf(fullpath, sizeof(fullpath), "%s/%s", path, entry->d_name);

        // Get the file status
        if (stat(fullpath, &filestat) < 0) {
            fprintf(stderr, "Error stating file %s\n", fullpath);
            continue;
        }

        // If the entry is a directory and the recursive option is enabled, call the function recursively
        if (S_ISDIR(filestat.st_mode) && recursive && strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")) {
            search_directory(fullpath, filename, uid, type, recursive);
        }

        // Check for filename match
        if (filename != NULL && strcmp(entry->d_name, filename) != 0) {
            continue;
        }

        // Check for UID match
        if (uid != NULL && atoi(uid) != filestat.st_uid) {
            continue;
        }

        // Check for the type of file based on the specified type
        if (type != NULL) {
            if (S_ISREG(filestat.st_mode) && strcmp(type, "regular") != 0) {
                continue;
            } else if (S_ISDIR(filestat.st_mode) && strcmp(type, "directory") != 0) {
                continue;
            }
            // Add more cases here for different file types if needed
        }

        // Print the full path of the matching file
        printf("%s\n", fullpath);
    }

    // Close the directory
    closedir(dir);
}

// Main function that parses command line arguments and calls the search_directory function
int main(int argc, char *argv[]) {
    // Check if the command line arguments are provided correctly
    if (argc < 2) {
        fprintf(stderr, "Usage: %s [directory] [options]\n", argv[0]);
        return 1;
    }

    const char *directory = argv[1];
    const char *filename = NULL;
    const char *uid = NULL;
    const char *type = NULL;
    int recursive = 0;

    // Parse the command line arguments
    for (int i = 2; i < argc; i++) {
        if (strcmp(argv[i], "-name") == 0) {
            filename = argv[i + 1];
            i++;
        } else if (strcmp(argv[i], "-R") == 0) {
            recursive = 1;
        } else if (strcmp(argv[i], "-uid") == 0) {
            uid = argv[i + 1];
            i++;
        } else if (strcmp(argv[i], "-type") == 0) {
            type = argv[i + 1];
            i++;
        }
    }

    // Call the search_directory function with the parsed arguments
    search_directory(directory, filename, uid, type, recursive);

    return 0;
}
