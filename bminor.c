#include <stdio.h>
#include <stdlib.h>
#include "./encoder.h"

int main(int argc, char *argv[]){
    int status = EXIT_SUCCESS;
    if (argc < 3){
        printf("Usage:  ./bminor --encode input.minor\n");
        return EXIT_FAILURE;
    }
    if (strcmp(argv[1],"--encode") == 0){
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
            if (input[strlen(input)-2] == '\n') {
                input[strlen(input)] = '\0';
            }
            printf("%s", input);
            printf("\n");
            int result = string_decode(input, decoded);
            if (result == 1) {
                printf("%s", decoded);
                printf("\n");
                string_encode(decoded, output);
                printf("%s", output);
                printf("\n");
            } else {
                printf("Invalid String: %s\n", input);
                printf("\n");
                status = EXIT_FAILURE;
            }
        }
        fclose(file);
    }
    
    return status;
}