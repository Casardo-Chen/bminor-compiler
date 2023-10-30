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

void param_list_print(struct param_list *a){
    if (!a) return;
    printf("%s: ", a->name);
    type_print(a->type);
    if (!a->next) return;
    printf(", ");
    param_list_print(a->next);
}
