#include <stdio.h>
#include <stdlib.h>
#include "./encoder.h"

int main(int argc, char *argv[]){
    int status = EXIT_SUCCESS;
    /* Encoder */
    if (argc == 3 && strcmp(argv[1],"--encode") == 0 ){
        /* initialize the file read stream and variables */
        FILE* file = fopen(argv[2], "r");
        if (!file){
            perror("Cannot open text\n");
            return EXIT_FAILURE;
        }
        char input[BUFSIZ];
        char decoded[BUFSIZ];
        char output[BUFSIZ];
        
        /* read in the test file */
        if(fgets(input, sizeof(input), file) != NULL){
            /* decode the code */
            if (input[0] == '\0') {
                printf("Empty Input\n");
            }
            if (input[strlen(input)-2] == '\n') {
                input[strlen(input)] = '\0';
            }
            printf("Input String    : %s", input);
            printf("\n");
            int result = string_decode(input, decoded);
            if (result == 1) {
                printf("Decoded String    : %s", decoded);
                printf("\n");
                string_encode(decoded, output);
                printf("Output String    : %s", output);
                printf("\n");
            } else {
                printf("Invalid String: %s\n", input);
                status = EXIT_FAILURE;
            }
        } else {
            printf("Null file descriptor\n");
            status = EXIT_FAILURE;
        }
        fclose(file);
    } else {
        printf("Usage:  ./bminor --encode input.minor\n");
        return EXIT_FAILURE;
    }
    return status;
}