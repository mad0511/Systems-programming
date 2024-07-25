#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <pwd.h>
#include <ctype.h>
#include <dirent.h>
#include <time.h>
#include <pthread.h>

//need a frequency
int frequency[26] = {0};
clock_t start_time, end_time; // Variables to store start and end times
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


void processFile(char * basePath) {
    //whole process file
    printf("Processing File: %s\n",basePath);

    //open the file
    FILE * stream = fopen(basePath ,"rb");

    char c = 0;
    //read the file char by char
    //will stop when C == EOF
    while ((c = fgetc(stream)) != EOF) {
        if (isalpha(c)) {
            //it's an alpha character (a-z)
            c = tolower(c);
            //make darn sure it's a-z
            //stoopid alt char sets
            if (c >= 'a' && c <= 'z') {
                // add mutex lock to execute the current process w/o other thread being executed
                pthread_mutex_lock(&mutex);
                //increment frequency
                frequency[c - 'a']++;
                pthread_mutex_unlock(&mutex);
            }
        }
    }

    //close the file
    fclose(stream);


}

void processDirectory(char * basePath) {
    //identify directory
    printf("Working with directory: %s\n",basePath);
    //open directory
    DIR * dir = opendir(basePath);
    //need a directory entry pointer
    struct dirent * dentry;
    //actually read an entry
    dentry = readdir(dir);
    //loop until dentry is null (no more directory entries)
    while (dentry != NULL) {
        char buffer[1000] = {0};
        strcat(buffer, basePath);
        strcat(buffer, "/");
        strcat(buffer,dentry->d_name);
        //printf("%s\n",buffer);
        if (dentry->d_type == DT_DIR) {
            if ((strcmp(dentry->d_name,".") != 0) && (strcmp(dentry->d_name,"..") != 0)) {
                pthread_t tid;
                pthread_create(&tid, NULL, processDirectory, strdup(buffer));
                pthread_join(tid, NULL);
                printf("The thread id for this directory is %d\n", tid);
            }
            
        } else if (dentry->d_type == DT_REG) {
            processFile(buffer);
        }
        
        //actually read an entry
        dentry = readdir(dir);
    }
    //close directory
    closedir(dir);
    pthread_exit(NULL);

}


int main() {
    char buffer[256];
    char * basePath = getcwd(buffer, 256);

    start_time = clock(); // Start the timer
    pthread_t tid;

    pthread_create(&tid, NULL, processDirectory, strdup(basePath));
    pthread_join(tid, NULL);

    end_time = clock(); // End the timer

    double total_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC; // Calculate total time

    for (int i = 0; i < 26; i++) {
        printf("%c: %d\n", ('a' + i), frequency[i]);
    }

    printf("Multi thread execution for each directory time: %f seconds\n", total_time);

    return 0;
}