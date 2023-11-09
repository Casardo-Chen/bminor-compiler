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
    return s;
}