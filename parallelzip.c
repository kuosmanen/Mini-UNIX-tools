#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
//tempfilenamesize is arbitrary. It could be made smaller too
#define TEMPFILENAMESIZE 50

typedef struct {
    FILE *file;
    char *tempFilename;
} ThreadData;


void * zip(void * arg) {
    ThreadData *data = (ThreadData *)arg;
    FILE *fp = data->file;
    //now we open the temp file we want to write the output to
    FILE *tempFile = fopen(data->tempFilename, "w");
    if (tempFile == NULL) {
        unlink(data->tempFilename);
        free(data->tempFilename);
        free(data);
        printf("pzip: cannot open file\n");
        exit(1);
    }
    int current, next;
    int count = 1;
    
    
    //reading the first character of the file
    current = fgetc(fp);
    while(current != EOF) {
        //here we count the characters and write them in binary
        next = fgetc(fp);
        if(current == next) {
            count ++;
        }
        else {
            //then we write the number to the tempfile using binary
            //in C, the size of int is 32 bits=4bytes
            fwrite(&count, sizeof(int), 1, tempFile);
            //then the character in ASCII
            fputc(current, tempFile);
            //then we move to the next character in the file
            current = next;
            //and reset the count
            count = 1;
        }
    }
    //after reading all lines, we close the file and temp file
    fclose(fp);
    fclose(tempFile);
    //then we free the memory of the struct we used
    //because we now have the tempfile we are going to use instead
    free(data->tempFilename);
    free(data);
    return NULL;
}


int main(int argc, char *argv[]) {
    pthread_t tid[argc-1];
    //we make a list for thread IDs for later reference

    if (argc < 2) {
        //if no arguments, return 1
        printf("pzip: file1 [file2 ...]\n");
        exit(1);
    }

    for ( int i = 1; i < argc; i++) {
        ThreadData *data = malloc(sizeof(ThreadData));
        //filenames start from argv[1], not argv[0]
        data->file = fopen(argv[i], "r");
        if (data->file == NULL) {
            printf("pzip: cannot open file\n");
            exit(1);
        }

        data->tempFilename = malloc(TEMPFILENAMESIZE);
        snprintf(data->tempFilename, TEMPFILENAMESIZE, "temp_%d.txt", i);

        pthread_create(& tid[i-1], NULL, zip, (void *)data);
    }

    //now the threads are doing their job
    //after the first one is done, we printf everything to stdout,
    //then move on to the next tempfile
     for (int i = 1; i < argc; i++) {
        pthread_join(tid[i-1], NULL);

        char tempFilename[TEMPFILENAMESIZE];
        snprintf(tempFilename, TEMPFILENAMESIZE, "temp_%d.txt", i);

        //writing the whole temp file to stdout
        FILE *tempFile = fopen(tempFilename, "r");
        if (tempFile) {
            //this can be made bigger
            char buffer[1024];
            size_t bytesRead;
            //we use a buffer so the file is read correctly with the binary and ASCII
            while ((bytesRead = fread(buffer, 1, sizeof(buffer), tempFile)) > 0) {
                fwrite(buffer, 1, bytesRead, stdout);
            }
            fclose(tempFile);
            //after writing, we delete the temp file
            unlink(tempFilename);
        }
        //then we move on to the next file
    }

    return(0);
}
