#include <stdio.h>
#include <stdlib.h>

//setting the buffer size to 1000 arbitralily
#define BUFFERSIZE 1000

int main(int argc, char *argv[]) {
    char* filename;
    char buffer[BUFFERSIZE];

    FILE *fp;

    if (argc < 2) {
        //if no arguments, return 0
        return(0);
    }
    //we loop through all the given argument files
    for ( int i = 1; i < argc; i++) {
        filename = argv[i];
        //we open the file in read mode
        fp = fopen(filename, "r");
        //check for errors in opening
        if (fp == NULL) {
            printf("my-cat: cannot open file\n");
            exit(1);
        }
        //then we read and also check if end of file NULL
        while(fgets(buffer, BUFFERSIZE, fp) != NULL) {
            printf("%s", buffer);
        }
        //after reading all lines, we close the file
        fclose(fp);
    }
    
    //after all files have been read, return 0
    return(0);
}