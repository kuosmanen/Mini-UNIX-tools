#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    char* filename;
    FILE *fp;
    int current, next;
    int count = 1;

    if (argc < 2) {
        //if no arguments, return 1
        printf("my-zip: file1 [file2 ...]\n");
        exit(1);
    }

    for ( int i = 1; i < argc; i++) {
        filename = argv[i];
        fp = fopen(filename, "r");
        //checking for errors in opening
        if (fp == NULL) {
            printf("my-zip: cannot open file\n");
            exit(1);
        }
        //reading the first character of the file
        current = fgetc(fp);
        while(current != EOF) {
            //here we count the characters and write them in binary
            next = fgetc(fp);
            if(current == next) {
                count ++;
            }
            else {
                //then we write the number to stdout using binary
                //in C, the size of int is 32 bits=4bytes
                fwrite(&count, sizeof(int), 1, stdout);
                //then the character in ASCII
                printf("%c", current);
                //then we move to the next character in the file
                current = next;
                //and reset the count
                count = 1;
            }
        }
        //after reading all lines, we close the file and move to the next
        fclose(fp);
    }
    return(0);
}