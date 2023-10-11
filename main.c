#include <stdio.h>
#include <stdlib.h>
#include "bminor_helper.h"

int main(int argc, char *argv[]){
    int status = EXIT_SUCCESS;
    /* I/O */
    if (argc < 3) {
        fprintf(stderr, "Usage:  ./bminor [--encode|--scan|--parse] input.bminor\n");
        return EXIT_FAILURE;
    }
    yyin = fopen(argv[2], "r");
    if (!yyin) {
        fprintf(stderr, "could not open %s!\n", argv[2]);
        return EXIT_FAILURE;
    }
    /* Encoder */
    if (strcmp(argv[1],"--encode") == 0 ){   
        status = encode();  
    } 
    /* Scanner */
    if (strcmp(argv[1],"--scan") == 0 ) {
        status = scan(); 
    } 
    /* Parser */
    if (strcmp(argv[1],"--parse") == 0 ) {
        status = parse(); 
        printf("status: %d\n", status);
    } 
    fclose(yyin);
    return status;
}