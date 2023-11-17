#include <stdio.h>
#include <stdlib.h>
#include "bminor_helper.h"

int main(int argc, char *argv[]){
    int status = EXIT_SUCCESS;
    /* I/O */
    if (argc < 3) {
        fprintf(stderr, "Usage:  ./bminor [--encode|--scan|--parse|--print] input.bminor\n");
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
    } 
    /* Print */
    if (strcmp(argv[1],"--print") == 0 ) {
        status = print(); 
    } 
    /* Resolve */
    if (strcmp(argv[1],"--resolve") == 0 ) {
        int resolve_err;
        resolve_err = resolve(); 
        status = resolve_err ? EXIT_FAILURE : EXIT_SUCCESS;
    }
     /* Typecheck */
    if (strcmp(argv[1],"--typecheck") == 0 ) {
        int type_err;
        type_err = typecheck(); 
        status = type_err ? EXIT_FAILURE : EXIT_SUCCESS;
    } 
    fclose(yyin);
    return status;
}