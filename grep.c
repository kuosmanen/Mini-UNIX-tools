#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//setting the buffer size to 1000 arbitralily
#define BUFFERSIZE 1000

int main(int argc, char *argv[]) {
    //checking command-line arguments
    if (argc == 1) {
        printf("my-grep: searchterm [file ...]\n");
        exit(1);
    }

    char* filename;
    char* keyword = argv[1];
    char *lineptr = NULL;
    FILE *fp;
    size_t n = 0;
    //setting lineptr to NULL and n to 0 so getline() allocates memory to the buffer later

    //If no file is given, we read stdin
    if (argc == 2) {
        char buffer[BUFFERSIZE];
        //NULL is not reached but could be changed to something else
        //so the user doesn't have to use ctrl+C to signal interrupt
        while(fgets(buffer, BUFFERSIZE, stdin) != NULL) {
            if (strstr(buffer, keyword) != NULL) {
                //null means keyword was not found in the line
                printf("%s", buffer);
            }
        }
    }


    for ( int i = 2; i < argc; i++) {
        filename = argv[i];
        fp = fopen(filename, "r");
        //check for errors in opening
        if (fp == NULL) {
            printf("my-grep: cannot open file\n");
            exit(1);
        }
        //then we read and also check if end of file -1
        while(getline(&lineptr, &n, fp) != -1) {
            if (strstr(lineptr, keyword) != NULL) {
                //null means keyword was not found in the line
                printf("%s", lineptr);
            }
        }
        //after reading all lines, we close the file
        fclose(fp);
    }

    //freeing the dynamically allocated memory
    free(lineptr);
    //after all files have been read, return 0
    return(0);
}