#include "decl.h"
#include "encoder.h"

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
                if (!param_list_eq(prev->type->params,d->type->params)){ // the parameter name can vary as long as the type is the same.
                    printf("resolve error: prototype of %s after the definition of %s global %d.\n", d->name, prev->name, prev->which);
                    resolve_error++;
                }
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
        if (d->symbol->proto){

        }
        scope_enter();
        param_list_resolve(d->type->params);
        // count how many variables in the scope, used in codegen
        if (d->code){ // function
            stmt_resolve(d->code);
            if (prev) { // previous prototype exists
                prev->total = scope_var_count();
                d->symbol = prev;
            } else { // definition is the reference
                d->symbol->total = scope_var_count();
            }
        } else {d->symbol->total = scope_var_count();} // proto
        scope_exit();
    }

    decl_resolve(d->next);
}

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
                printf("type error: non-constant initializers for global variables ");
                expr_print(d->value);
                printf(" is invalid.\n");
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

void decl_codegen_global(struct decl *d) {
    if(!d) return;
    switch (d->type->kind){
        case TYPE_ARRAY:
            if (d->type->subtype->kind != TYPE_INTEGER) {
                printf("codegen error: array not implemented.\n");
                exit(1);
            }
            fprintf(outfile, ".data\n");
            int l;
            if (d->value) {
                l = label_create();
                fprintf(outfile, "%s:\n", string_label_name(l));
                struct expr* arg = d->value->left;
                while (arg) {
                    fprintf(outfile, "\t.quad %d\n", arg->left->literal_value);
                    arg = arg->right;
                }
            }
            fprintf(outfile, ".globl %s\n", d->name);
            fprintf(outfile, "%s:\n", d->name);
            fprintf(outfile, "\t.quad %s\n", d->value ? string_label_name(l) : "0");
            break;
        case TYPE_BOOLEAN:
        case TYPE_INTEGER:
        case TYPE_CHARACTER:
            d->value = d->value ? d->value->left : d->value; 
            fprintf(outfile, ".data\n");
            fprintf(outfile, ".globl %s\n", d->name);
            fprintf(outfile, "%s:\n",d->name);
            fprintf(outfile, "\t.quad %d\n", d->value ? d->value->literal_value : 0);
            break;
        case TYPE_STRING:{
            d->value = d->value ? d->value->left : d->value; 
            fprintf(outfile, ".data\n");
            int l;
            if (d->value){
                l = label_create();
                fprintf(outfile, "%s:\n", string_label_name(l));
                char es[BUFSIZ];
                string_encode(d->value->string_literal, es);
                fprintf(outfile, "\t.string %s\n", es);
            }
            fprintf(outfile, ".globl %s\n", d->name);
            fprintf(outfile, "%s:\n",d->name);
            fprintf(outfile, "\t.quad %s\n", d->value ? string_label_name(l) : "0");
            break;
        }
        case TYPE_FUNCTION:{
            fprintf(outfile, ".text\n");
            fprintf(outfile, ".globl %s\n", d->name);
            if (d->code){
                fprintf(outfile, "%s:\n", d->name); 
                fprintf(outfile, "\tPUSHQ %%rbp\n");
                fprintf(outfile, "\tMOVQ %%rsp, %%rbp\n");
                /* param codegen */
                int num_args = param_list_codegen(d->type->params);
                /* local vars */
                fprintf(outfile, "\tSUBQ $%d, %%rsp\n", (d->symbol->total - num_args) * 8);

                /* push all the callee saved registers */
                fprintf(outfile, "\tPUSHQ %%rbx\n");
                fprintf(outfile, "\tPUSHQ %%r12\n");
                fprintf(outfile, "\tPUSHQ %%r13\n");
                fprintf(outfile, "\tPUSHQ %%r14\n");
                fprintf(outfile, "\tPUSHQ %%r15\n");

                /* body */
                stmt_codegen(d->code, d->name);

                /* pop all the callee saved register */
                fprintf(outfile, ".%s_epilogue:\n", d->name);
                fprintf(outfile, "\tPOPQ %%r15\n");
                fprintf(outfile, "\tPOPQ %%r14\n");
                fprintf(outfile, "\tPOPQ %%r13\n");
                fprintf(outfile, "\tPOPQ %%r12\n");
                fprintf(outfile, "\tPOPQ %%rbx\n");

                /* return */
                fprintf(outfile, "\tMOVQ %%rbp, %%rsp\n");
                fprintf(outfile, "\tPOPQ %%rbp\n");
                fprintf(outfile, "\tRET\n");
            }
            break;
        }
        case TYPE_FLOAT: // TODO: reserved for float
            printf("codegen error: floating not supported.\n");
            exit(1);
            break;
        default:
            printf("codegen error: unknown type kind %d.\n", d->type->kind);
            break;
    }

    decl_codegen_global(d->next);
}

void decl_codegen_local(struct decl *d) {
    if (!d) return;
    if (d->value) {
        expr_codegen(d->value);
        fprintf(outfile, "\tMOVQ %%%s, %s\n", scratch_name(d->value->reg), symbol_codegen(d->symbol));
        scratch_free(d->value->reg);
    }
    decl_codegen_local(d->next);
}