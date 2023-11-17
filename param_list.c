#include "param_list.h"

struct param_list * param_list_create( const char *name, struct type *type, struct param_list *next ){
    struct param_list * p = (struct param_list *) malloc (sizeof(struct param_list));
    if (!p) {
        printf("print error: failed to allocate memory for param_list.\n");
        exit(1);
    }

    p->name = strdup(name);
    p->type = type;
    p->next = next;

    return p;
}

void param_list_print(struct param_list *p){
    if (!p) return;
    printf("%s: ", p->name);
    type_print(p->type);
    if (!p->next) return;
    printf(", ");
    param_list_print(p->next);
}

/*
    enter a new variable declaration for each parameter of a
    function, so that those definitions are available to the code of a function
*/
void param_list_resolve(struct param_list *p){
    if(!p) return;
    p->symbol = symbol_create(SYMBOL_PARAM, p->type, p->name);
    if (!scope_lookup_current(p->name)){
        scope_bind(p->name, p->symbol);
    } else {
        printf("resolve error: duplicate %s is invalid.\n", p->name);
        resolve_error++;
    }
    param_list_resolve(p->next);
}

int param_list_eq(struct param_list *a, struct param_list *b) {
    // recursively check if all params are the same
    if ((!a && b) || (a && !b)) return 0;
    else if (!a && !b) return 1; 
    if (type_eq(a->type, b->type)) {
        return param_list_eq(a->next, b->next);
    }
    return 0;
}

struct param_list* param_list_copy(struct param_list *p) {
    if (!p) return NULL;
    return param_list_create(p->name, type_copy(p->type), param_list_copy(p->next));
}

void param_list_delete(struct param_list *p){
    if (!p) return;
    type_delete(p->type);
    param_list_delete(p->next);
    free(p);
}

void param_list_valid(struct param_list *p) {
    if (!p) return;
    if (p->type->kind == TYPE_FUNCTION) {
        printf("type error: parameters cannot contain type function.\n");
        type_error++;
    }
    param_list_valid(p->next);
}