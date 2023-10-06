#include <stdio.h>
#include <stdlib.h>
#include "encoder.h"
#include "scanner_helper.h"

int main(int argc, char *argv[]){
    int status = EXIT_SUCCESS;
    /* I/O */
    if (argc < 3) {
        fprintf(stderr, "Usage:  ./bminor [--encode|--scan] input.bminor\n");
        return EXIT_FAILURE;
    }
    yyin = fopen(argv[2], "r");
    if (!yyin) {
        fprintf(stderr, "could not open %s!\n", argv[2]);
        return EXIT_FAILURE;
    }
    /* Encoder */
    if (strcmp(argv[1],"--encode") == 0 ){    
        char input[BUFSIZ];
        char decoded[BUFSIZ];
        char output[BUFSIZ];
        
        /* read in the test file */
        if(fgets(input, sizeof(input), yyin) != NULL){
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
    } 

    /* Scanner */
    if (strcmp(argv[1],"--scan") == 0 ) {
        status = scan(yyin); 
    } 
    fclose(yyin);
    return status;
}