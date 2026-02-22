#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc > 3) {
        fprintf(stderr, "usage: reverse <input> <output>\n");
        exit(1);
    }

    //By default, the input is stdin unless it's specified later.
    //The user needs to make an end of line signal (CTRL+D) in order to get the lines reversed,
    //if no command line arguments are given.
    FILE *input = stdin;
    FILE *output = stdout;
    //line pointer is NULL and line_size is 0 so getline() allocates needed memory
    char *line = NULL;
    size_t line_size = 0;
    size_t len_lines = 0;
    char **lines_list = NULL;

    if (argc >= 2) {
        input = fopen(argv[1], "r");
        if (input == NULL) {
            fprintf(stderr, "error: cannot open file '%s'\n", argv[1]);
            exit(1);
        }
        if (argc == 3) {
            if (strcmp(argv[1], argv[2]) == 0) {
                fprintf(stderr, "Input and output file must differ\n");
                exit(1);
            }
            output = fopen(argv[2], "w");
            if (output == NULL) {
                fprintf(stderr, "error: cannot open file '%s'\n", argv[2]);
                exit(1);
            }
        }
    }
    

    //getline will allocate a buffer of correct size when line pointer is NULL and size is 0
    //later when the buffer is reused, getline() will increase the buffersize if needed
    while (getline(&line, &line_size, input) != -1) {
        //resizing the list to store another line
        lines_list = realloc(lines_list, (len_lines + 1) * sizeof(char *));
        if (lines_list == NULL) {
            fprintf(stderr, "realloc failed\n");
            //freeing the memory incase of realloc failure
            free(line);
            for (size_t i = 0; i < len_lines; i++) free(lines_list[i]);
            free(lines_list);
            exit(1);
        }
        //copying the line pointer to our list of line pointers so we can reuse the buffer
        lines_list[len_lines] = strdup(line);
        if (lines_list[len_lines] == NULL) {
            //freeing the memory incase of strdup failure
            fprintf(stderr, "strdup failed\n");
            free(line);
            for (size_t i = 0; i < len_lines; i++) free(lines_list[i]);
            free(lines_list);
            exit(1);
        }
        len_lines++;
    }

    //printing reverse order of lines
    for (size_t i = len_lines; i > 0; i--) {
        fprintf(output, "%s", lines_list[i - 1]);
    }

    //freeing the memory of the lines in the list
    for (size_t i = 0; i < len_lines; i++) {
        free(lines_list[i]);
    }
    free(lines_list);
    free(line);

    //finally closing the input and output files
    if (input != stdin) fclose(input);
    if (output != stdout) fclose(output);

    return 0;
}
