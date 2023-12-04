#include "symbol.h"

struct symbol * symbol_create( symbol_t kind, struct type *type, char *name ){
    struct symbol *s = (struct symbol *) malloc(sizeof(struct symbol));
    if(!s){
        printf("resolve error: failed to allocate memory for symbol.\n");
        exit(1);
    }
    s->kind = kind;
	s->type = type;
	s->name = name;
	s->which = 0;
    s->proto = 0;
    s->modified = 0;
    s->total = 0;
    return s;
}

const char *  symbol_codegen( struct symbol *s){
    switch (s->kind) {
        case SYMBOL_GLOBAL:
            return s->name;
        case SYMBOL_LOCAL:
        case SYMBOL_PARAM:{
            char str[BUFSIZ];
            sprintf(str, "%d(%%rbp)", -8 * (s->which + 1));
            return strdup(str);
        }
        default:
            fprintf(stderr, "codegen error: unknown symbol type %d\n", s->kind);
            exit(1);
    }
}