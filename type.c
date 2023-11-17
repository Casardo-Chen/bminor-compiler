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
            printf("invalid type kind\n");
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

struct type * type_copy(struct type *t){
    if (!t) return NULL;
    return type_create(t->kind, type_copy(t->subtype), param_list_copy(t->params), expr_copy(t->val));
}

void type_delete(struct type *t){
    if (!t) return;
    type_delete(t->subtype);
    param_list_delete(t->params);
    expr_delete(t->val);
    free(t);
}

void type_valid(struct type *t, int arr_internal) {
    if (!t) return;
    switch (t->kind) {
        case TYPE_BOOLEAN:
        case TYPE_INTEGER:
        case TYPE_CHARACTER:
        case TYPE_FLOAT:
        case TYPE_STRING:
        case TYPE_VOID:
            return;
        case TYPE_ARRAY:
            if (t->val->kind != EXPR_INT_LIT) {
                printf("type error: array size (");
                expr_print(t->val);
                printf(") is not an integer literal\n");
                type_error++;
            }
            type_valid(t->subtype, 1); // 1: if we are checking if it is an array of function
            return;
        case TYPE_FUNCTION:
            if (arr_internal) {
                printf("type error: array cannot contain type function\n");
                type_error++;
            } else {
                if (t->subtype->kind == TYPE_FUNCTION) {
                    printf("type error: function cannot be the return value of a function\n");
                    type_error++;
                }
                param_list_valid(t->params);
            }
            return;
        default:
            return;
    }
}
