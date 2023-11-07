#include "decl.h"

struct decl * decl_create( const char *name, struct type *type, struct expr *value, struct stmt *code, struct decl *next ){
    struct decl *d = (struct decl *) malloc(sizeof(struct decl));
    if(!d){
        printf("print error: failed to allocate memory for decl.\n");
        exit(1);
    }
    d->name = strdup(name);
    d->type = type;
    d->value = value;
    d->code = code;
    d->next = next;

    return d;
}

void decl_print(struct decl *d, int indent){
    if (!d) return;
    /*  E.g.
        b: boolean;
        x: integer = 3;
        y: array [2] boolean = {true,false};
        writechar: function void ( c: char );
    */
    /* indent */
    indent_print(indent);

    /* identifier */
    printf("%s: ", d->name);

    /* type */
    type_print(d->type);

    /* expr*/ 
    if (d->value) {
        printf(" = ");
        expr_print(d->value);
        printf(";");
    }
    /* stmt */ 
    else if (d->code) {
        printf(" = ");
        stmt_print(d->code, indent);
    } 
    else {
        printf(";");
    }
    printf("\n");

    /*next declaration */
    decl_print(d->next, indent);
}

void indent_print(int indent){
    for (int i = 0; i < indent * 4; i++){
        printf(" ");
    }
}

void decl_resolve( struct decl *d ){
    if (!d) return;
    symbol_t kind = scope_level() > 1 ? SYMBOL_LOCAL : SYMBOL_GLOBAL;
    d->symbol = symbol_create(kind, d->type,d->name);
    expr_resolve(d->value);
    scope_bind(d->name,d->symbol);
    if(d->code) {
        scope_enter();
        param_list_resolve(d->type->params);
        stmt_resolve(d->code);
        scope_exit();
    }
    decl_resolve(d->next);
}
