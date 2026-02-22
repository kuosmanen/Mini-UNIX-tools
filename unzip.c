#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    char* filename;
    FILE *fp;
    int count;
    char character;
    //count stores the non-binary integer and character stores the ASCII character

    if (argc < 2) {
        //if no arguments, return 1
        printf("my-unzip: file1 [file2 ...]\n");
        exit(1);
    }

    for ( int i = 1; i < argc; i++) {
        filename = argv[i];
        fp = fopen(filename, "r");
        //checking for errors in opening
        if (fp == NULL) {
            printf("my-unzip: cannot open file\n");
            exit(1);
        }
        while(fread(&count, sizeof(int), 1, fp) != 0) {
            //we read the size of a single binary integer and store that in the count
            //EOF is indicated by fread returning a 0

            //We also read the character from the file
            fread(&character, sizeof(char), 1, fp);
            //then the character in ASCII is printed as many times as the count states
            for (int i=0; i < count; i++) {
                printf("%c", character);
            }
        }
        //after reading all lines, we close the file and move to the next
        fclose(fp);
    }
    return(0);
}