#include "type.h"

struct type * type_create( type_t kind, struct type *subtype, struct param_list *params, struct expr *val ){
    struct type * t = (struct type *) malloc (sizeof(struct type));
    if (!t) {
        printf("print error: failed to allocate memory for type.\n");
        exit(1);
    }
    t->kind = kind;
    t->subtype = subtype;
    t->params = params;
    t->val = val;
    
    return t;

}

void type_print( struct type *t ){
    if(!t) return;
    switch (t->kind){
        case TYPE_VOID:
            printf("void");
            break;
        case TYPE_BOOLEAN:
            printf("boolean");
            break;
        case TYPE_CHARACTER:
            printf("char");
            break;
        case TYPE_FLOAT:
            printf("float");
            break;
        case TYPE_INTEGER:
            printf("integer");
            break;
        case TYPE_STRING:
            printf("string");
            break;
        case TYPE_ARRAY:
            printf("array [");
            expr_print(t->val);
            printf("] ");
            type_print(t->subtype);
            break;
        case TYPE_FUNCTION:
            printf("function ");
            type_print(t->subtype);
            printf(" ( ");
            param_list_print(t->params);
            printf(" )");
            break;
        default:
            printf("print error: invalid type kind found.\n");
            exit(1);
            break;
    }
    

}

int type_eq(struct type *a, struct type *b){
    if (a->kind == b->kind) {
        if(a->kind == TYPE_ARRAY && type_eq(a->subtype, b->subtype)){
            return a->val->literal_value == b->val->literal_value;
        } 
        if(a->kind == TYPE_FUNCTION && type_eq(a->subtype, b->subtype)){
            return param_list_eq(a->params, b->params);
        }
        return 1;
    }
    return 0;
}
