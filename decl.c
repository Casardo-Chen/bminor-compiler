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

    /* expr */ 
    expr_resolve(d->value);
        
    /* scope binding */
    /* if d is a prototype of a function */ 
    if (d->type->kind == TYPE_FUNCTION && !d->code) {
        d->symbol->proto = 1;
    }
    /* check for the previous declaration of function */ 
    struct symbol * prev = scope_lookup_current(d->name);
    /* a previous declaration of this function was found */ 
    if (prev && d->type->kind == TYPE_FUNCTION) { 
        if (prev->type->kind != TYPE_FUNCTION){
            printf("resolve error: different prototypes of %s is invalid.\n", d->name);
            resolve_error++;
        }
        else {
            /* prev: proto | curr: proto */ 
            if (prev->proto && d->symbol->proto) {
                if (!type_eq(prev->type, d->type)) {
                    printf("resolve error: different prototypes of %s is invalid.\n", d->name);
                    resolve_error++;
                }
            } 

            /* prev: proto | curr: !proto */
            else if (prev->proto && !d->symbol->proto) {
                if (type_eq(prev->type, d->type)) {
                    printf("%s defines prototype global %s.\n", d->name, prev->name);
                } else {
                    printf("resolve error: definition of %s does not match prototype %s global %d.\n", d->name, prev->name, prev->which);
                    resolve_error++;
                } 
                prev->proto = 0;
            }

            /* prev: !proto | curr: proto */
            else if (!prev->proto && d->symbol->proto) {
                printf("resolve error: prototype of %s after the definition of %s global %d.\n", d->name, prev->name, prev->which);
                resolve_error++;
            }

            /* prev: !proto | curr: !proto */
            else if (!prev->proto && !d->symbol->proto) {
                printf("resolve error: duplicate definitions of %s is invalid.\n", d->name);
                resolve_error++;
            }
        }

    } 
    /* a previous declaration of this decl was found */ 
    else if (prev) {
        printf("resolve error: duplicate definitions of %s in the same scope is invalid.\n", d->name);
        resolve_error++;
    }
    /* good case */
    else {
        scope_bind(d->name,d->symbol);
    }
    
    /* function */
    if(d->type->kind == TYPE_FUNCTION) {
        scope_enter();
        param_list_resolve(d->type->params);
        if (d->code) stmt_resolve(d->code);
        scope_exit();
    }

    decl_resolve(d->next);
}


// void decl_typecheck( struct decl *d ){
//      if (!d) return;
//      if (d->type->kind == TYPE_FUNCTION) {
//         /* Function cannot return array or another function */
//         if (d->type->subtype->kind == TYPE_ARRAY || d->type->subtype->kind == TYPE_FUNCTION) {
//             printf("[ERROR]    Type error: function '%s' does not support return type ", d->name);
//             type_print(d->type->subtype);
//             printf("\n");
//             type_error++;
//         }
//         /* Check parameters */
//         for (struct param_list* param = d->type->params; param; param = param->next) {
//             if (param->type->kind == TYPE_FUNCTION) {
//                 /* Parameters cannot contain function type */
//                 printf("Type error: declaring parameter '%s' of function '%s' as function type is not supported", param->name, d->name);
//                 type_error++;
//             } else if (param->type->kind == TYPE_ARRAY) {
//                 /* Array param cannot have declared length */
//                 if (param->type->val) {
//                     printf("Type error: array parameter '%s' of function '%s' must be declared with empty length", param->name, d->name);
//                     type_error++;
//                 }
//             }
//         }
//         /* Check functions defined in local space */
//         if (d->symbol->kind == SYMBOL_LOCAL) {
//             printf("Type error: declaring function '%s' in local scope is not supported", d->name);
//             type_error++;
//         }
//     }

//     if (d->type->kind == TYPE_ARRAY) {
//         /* IMPORTANT: Check array recursively since we allow nested arrays */
//         struct type* curr = d->type;
//         while (curr->kind == TYPE_ARRAY) {
//             /* Array of functions */
//             if (curr->subtype->kind == TYPE_FUNCTION) {
//                 printf("Type error: declaring array '%s' as array of functions is not supported", d->name);
//                 type_error++;
//             }
//             /* Array length */
//             struct type* len_type = expr_typecheck(curr->val);
//             if (d->symbol->kind == SYMBOL_GLOBAL) {
//                 if (!len_type) {
//                     printf("Type error: global array '%s' must be declared with constant integer length", d->name);
//                     type_error++;
//                 } else if (curr->val->kind != EXPR_INT_LIT) {
//                     printf("[ERROR]    Type error: expect declared length of global array '%s' to be constant integer, but found ", d->name);
//                     type_print(len_type);
//                     printf("\n");
//                     type_error++;
//                 }
//             } else if (d->symbol->kind == SYMBOL_LOCAL) {
//                 /* If in local scope, non-integer evaluation */
//                 if (!len_type) {
//                     printf("Type error: local array '%s' must be declared with length that evaluates to integer", d->name);
//                     type_error++;
//                 } else if (len_type->kind != TYPE_INTEGER) {
//                     printf("[ERROR]    Type error: expect declared length of local array '%s' to be integer, but found ", d->name);
//                     type_print(d->type->val->symbol->type);
//                     printf("\n");
//                     type_error++;
//                 }
//             }
//             /* Recurse to the subtype of the array */
//             curr = curr->subtype;
//         }
//     }

//      /* expression */
//     struct type *t;
//     t = expr_typecheck(d->value);
//     if(t) {
//         // check if the declaration is a function or void, which should not have values
//         if(t->kind == TYPE_VOID){
//             printf("type error: invalid expression type of void in assignment.\n");
//             type_error++;
//         }
//         // array
//         if(t->kind == TYPE_ARRAY){

//         }
//         // function
//         if(t->kind == TYPE_FUNCTION){
//             printf("type error: invalid expression type of function in assignment.\n");
//             type_error++;
//         }
//         // Non-constant initializers for global variables.
//         if(d->symbol->kind == SYMBOL_GLOBAL){
//             if (!expr_const(d->value)){
//                 printf("type error: non-constant initializers for global variables is invalid.\n");
//                 type_error++;
//             }
//         } else {
//             if (t->kind == TYPE_ARRAY && d->value->kind == EXPR_BR) {
//                 printf("type error: array initializers in non-global scope (");
//                 expr_print(d->value);
//                 printf(") is invalid\n");
//                 type_error++;
//             } 
//             if (t->kind == TYPE_FUNCTION) {
//                 printf("type error: function declaration in non-global scope (");
//                 expr_print(d->value);
//                 printf(") is invalid\n");
//                 type_error++;
//             }
//         }
//         // check if the type matches the one in declaration
//         if(!type_eq(t,d->symbol->type)) {
//             printf("type error: declaration of ");
//             type_print(d->symbol->type);
//             printf(" for %s does not match expression type ", d->name);
//             type_print(t);
//             printf("\n");
//             type_error++;
//         }
       
//     }
//      // type_delete(t);
//     /* function */
//     stmt_typecheck(d->code, d);
//     decl_typecheck(d->next);
// }

void decl_typecheck( struct decl *d ){
    if (!d) return;
    type_valid(d->type, 0);
    struct type *t;
    t = expr_typecheck(d->value);
    /* expression */
    if(t) {
        // check if the declaration is a function or void, which should not have values
        if(t->kind == TYPE_VOID){
            printf("type error: invalid expression type of void in assignment.\n");
            type_error++;
        }
        // function
        if(t->kind == TYPE_FUNCTION){
            printf("type error: invalid expression type of function in assignment.\n");
            type_error++;
        }
        // Non-constant initializers for global variables.
        if(d->symbol->kind == SYMBOL_GLOBAL){
            if (!expr_const(d->value)){
                printf("type error: non-constant initializers for global variables is invalid.\n");
                type_error++;
            }
        } else {
            if (t->kind == TYPE_ARRAY && d->value->kind == EXPR_BR) {
                printf("type error: array initializers in non-global scope (");
                expr_print(d->value);
                printf(") is invalid\n");
                type_error++;
            } 
            if (t->kind == TYPE_FUNCTION) {
                printf("type error: function declaration in non-global scope (");
                expr_print(d->value);
                printf(") is invalid\n");
                type_error++;
            }
        }
        // check if the type matches the one in declaration
        if(!type_eq(t,d->type)) {
            printf("type error: declaration of ");
            type_print(d->symbol->type);
            printf(" for %s does not match expression type ", d->name);
            type_print(t);
            printf("\n");
            type_error++;
        }
       
    }
    type_delete(t);
    /* function */
    stmt_typecheck(d->code, d);
    decl_typecheck(d->next);
}
