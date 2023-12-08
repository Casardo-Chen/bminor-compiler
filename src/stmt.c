#include "stmt.h"

struct stmt * stmt_create( stmt_t kind, struct decl *decl, struct expr *init_expr, struct expr *expr, struct expr *next_expr, struct stmt *body, struct stmt *else_body, struct stmt *next ){
    struct stmt * s = (struct stmt *) malloc (sizeof(struct stmt));
    if (!s) {
        printf("print error: failed to allocate memory for stmt.\n");
        exit(1);
    }
    s->kind = kind;
    s->decl = decl;
    s->init_expr = init_expr;
    s->expr = expr;
    s->next_expr = next_expr;
    s->body = body;
    s->else_body = else_body;
    s->next = next;
    s->pre_indent = 1;
    s->post_newline = 1;

    return s;
}


void stmt_print( struct stmt *s, int indent ){
    if (!s) return;
    switch (s->kind) {
        case STMT_DECL:
            decl_print(s->decl, indent);
            break;

        case STMT_EXPR:
            indent_print(indent);
            expr_print(s->expr);
            printf(";\n");
            break;

        case STMT_IF_ELSE:
            if (s->pre_indent) indent_print(indent);
            printf("if (");
            expr_print(s->expr);
            printf(") ");
            s->body = stmt_wrap(s->body);
            s->body->pre_indent = 0;
            if (s->else_body) s->body->post_newline = 0;
            stmt_print(s->body, indent);
            if (s->else_body){
                printf(" else ");
                if (s->else_body->kind != STMT_IF_ELSE){
                    s->else_body = stmt_wrap(s->else_body);
                    s->else_body->pre_indent = 0;
                } else {
                    s->else_body->pre_indent = 0;
                    s->else_body->post_newline = 0;
                }
                stmt_print(s->else_body, indent);
            }
            break;

        case STMT_FOR:
            indent_print(indent);
            printf("for (");
            expr_print(s->init_expr);
            printf("; ");
            expr_print(s->expr);
            printf("; ");
            expr_print(s->next_expr);
            printf(") ");
            s->body = stmt_wrap(s->body);
            s->body->pre_indent = 0;
            stmt_print(s->body, indent);
            break;

        case STMT_PRINT:
            indent_print(indent);
            printf("print");
            if(s->expr){
                printf(" ");
                expr_print(s->expr);
            }
            printf(";\n");
            break;

        case STMT_RETURN:
            indent_print(indent);
            printf("return");
            if(s->expr){
                printf(" ");
                expr_print(s->expr);
            }
            printf(";\n");
            break;

        case STMT_BR:
            if (s->body && s->body->kind == STMT_BR && !s->body->next) {
                stmt_print(s->body, indent);
                break;
            }
            // with braces
            if (s->pre_indent) indent_print(indent);
            printf("{\n");
            stmt_print(s->body, indent + 1);
            indent_print(indent);
            printf("}");
            if (s->post_newline) printf("\n");
            break;

        default:
            printf("print error: invalid statement type.\n");
            exit(1);
            break;
    }
    stmt_print(s->next, indent);
}


struct stmt * stmt_wrap( struct stmt *s ) {
    if (s->kind != STMT_BR) {
        return stmt_create(STMT_BR, 0, 0, 0, 0, s, 0, 0);
    } 
    return s;
}

void stmt_resolve( struct stmt *s ){
    if(!s) return;
    switch (s->kind){
        case STMT_DECL:
            decl_resolve(s->decl);
            break;
        case STMT_EXPR:
            expr_resolve(s->expr);
            break;
        case STMT_RETURN:
            expr_resolve(s->expr);
            break;
        case STMT_PRINT:
            expr_resolve(s->expr);
            break;
        case STMT_IF_ELSE:
            expr_resolve(s->expr);
            scope_enter();
            stmt_resolve(s->body);
            scope_exit();
            scope_enter();
            stmt_resolve(s->else_body);
            scope_exit();
            break;
        case STMT_FOR:
            expr_resolve(s->init_expr);
            expr_resolve(s->expr);
            expr_resolve(s->next_expr);
            scope_enter();
            stmt_resolve(s->body);
            scope_exit();
            break;
        case STMT_BR:
            scope_enter();
            stmt_resolve(s->body);
            scope_exit();
            break;
        default:
            printf("resolve error: undefined statement kind.\n");
            resolve_error++;
            exit(1);
    }
    stmt_resolve(s->next);
}


void stmt_typecheck( struct stmt *s, struct decl *d ){
    if (!s) return;
    struct type *t;
    switch(s->kind) {
        case STMT_EXPR:
            expr_typecheck(s->expr);
            break;
        case STMT_DECL:
            decl_typecheck(s->decl);
            break;
        case STMT_IF_ELSE:
            t = expr_typecheck(s->expr);
            if(t->kind!=TYPE_BOOLEAN) {
                printf("type error: condition in the if statement is ");
                type_print(t);
                printf(" (");
                expr_print(s->expr);
                printf("), which has to be boolean.\n");
                type_error++;
            } 
            type_delete(t);
            stmt_typecheck(s->body, d);
            stmt_typecheck(s->else_body, d);
            break;
        case STMT_RETURN:{
            struct type *func_type = d->type->subtype;
            t = expr_typecheck(s->expr);
            if (!t) break;
            if (!type_eq(t, func_type)) {
                printf("type error: cannot return a ");
                type_print(t);
                printf(" (");
                expr_print(s->expr);
                printf(") in a function (%s) that returns ", d->name);
                type_print(func_type);
                printf("\n");
                type_error++;
            }
            type_delete(t);
            break;
        }
        case STMT_PRINT:
            expr_typecheck(s->expr);
            break;
        case STMT_FOR:{
            struct type *init_type = expr_typecheck(s->init_expr);
            if (init_type && init_type->kind != TYPE_INTEGER){
                printf("type error: the first condition in the for loop statement is ");
                type_print(init_type);
                printf(" (");
                expr_print(s->init_expr);
                printf("), which has to be an integer.\n");
                type_error++;
            }
            struct type *mid_type = expr_typecheck(s->expr);
            if (mid_type && mid_type->kind != TYPE_BOOLEAN){
                printf("type error: the second condition in the for loop statement is ");
                type_print(mid_type);
                printf(" (");
                expr_print(s->expr);
                printf("), which has to be a boolean.\n");
                type_error++;
            }
            struct type *next_type = expr_typecheck(s->next_expr);
            if (next_type && next_type->kind != TYPE_INTEGER){
                printf("type error: the third condition in the for loop statement is ");
                type_print(next_type);
                printf(" (");
                expr_print(s->next_expr);
                printf("), which has to be an integer.\n");
                type_error++;
            }
            type_delete(init_type);
            type_delete(mid_type);
            type_delete(next_type);
            stmt_typecheck(s->body, d);
            break;
            }     
        case STMT_BR:
            stmt_typecheck(s->body, d);
            break;
        default:
            printf("type error: undefined statement kind.\n");
            type_error++;
            exit(1);
    }
    stmt_typecheck(s->next, d);
}

void stmt_codegen(struct stmt *s, const char *function_name){
    if(!s) return;
    switch(s->kind){
        case STMT_DECL:
            decl_codegen_local(s->decl);
            break;
        case STMT_EXPR:
            expr_codegen(s->expr);
            scratch_free(s->expr->reg);
            break;
        case STMT_IF_ELSE:{
            int else_label = label_create();
            int done_label = label_create();
            expr_codegen(s->expr);
            fprintf(outfile, "\tCMP $0, %%%s\n",scratch_name(s->expr->reg));
            scratch_free(s->expr->reg);
            fprintf(outfile, "\tJE %s\n",label_name(else_label));
            stmt_codegen(s->body, function_name);
            fprintf(outfile, "\tJMP %s\n",label_name(done_label));
            fprintf(outfile, "%s:\n",label_name(else_label));
            stmt_codegen(s->else_body, function_name);
            fprintf(outfile, "%s:\n",label_name(done_label));
            break;
        }
        case STMT_RETURN:
            expr_codegen(s->expr);
            fprintf(outfile, "\tMOV %%%s, %%rax\n",scratch_name(s->expr->reg));
            fprintf(outfile, "\tJMP .%s_epilogue\n", function_name);
            scratch_free(s->expr->reg);
            break;
        case STMT_FOR:{
            int top_label = label_create();
            int done_label = label_create();
            if (s->init_expr){
                expr_codegen(s->init_expr);
                scratch_free(s->expr->reg);
            }
            fprintf(outfile, "%s:\n", label_name(top_label));
            if (s->expr){
                expr_codegen(s->expr);
                fprintf(outfile, "\tCMPQ $0, %%%s\n", scratch_name(s->expr->reg));
                scratch_free(s->expr->reg);
                fprintf(outfile, "\tJE %s\n", label_name(done_label));
                
            }
            stmt_codegen(s->body, function_name);
            if (s->next_expr){
                    expr_codegen(s->next_expr);
                    scratch_free(s->expr->reg);
            }
            fprintf(outfile, "\tJMP %s\n", label_name(top_label));
            fprintf(outfile, "%s:\n", label_name(done_label));
            break;
        }
        case STMT_PRINT:
            for (struct expr* curr = s->expr; curr; curr = curr->right) {
                struct type* t = expr_typecheck(curr->left);
                struct expr* e;
                switch (t->kind) {
                    case TYPE_BOOLEAN:
                        e = expr_create(EXPR_CALL, expr_create_name("print_boolean"), expr_create(EXPR_ARG, curr->left, NULL, 8), 8);
                        expr_codegen_fcall(e);
                        scratch_free(e->reg);
                        break;
                    case TYPE_CHARACTER:
                        e = expr_create(EXPR_CALL, expr_create_name("print_character"), expr_create(EXPR_ARG, curr->left, NULL, 8), 8);
                        expr_codegen_fcall(e);
                        scratch_free(e->reg);
                        break;
                    case TYPE_INTEGER:
                        e = expr_create(EXPR_CALL, expr_create_name("print_integer"), expr_create(EXPR_ARG, curr->left, NULL, 8), 8);
                        expr_codegen_fcall(e);
                        scratch_free(e->reg);
                        break;
                    case TYPE_STRING:
                        e = expr_create(EXPR_CALL, expr_create_name("print_string"), expr_create(EXPR_ARG, curr->left, NULL, 8), 8);
                        expr_codegen_fcall(e);
                        scratch_free(e->reg);
                        break;
                    case TYPE_FLOAT:
                        printf("curr->left val %s ",curr->left->string_literal);
                        printf("curr->left type %d\n",curr->left->kind);
                        e = expr_create(EXPR_CALL, expr_create_name("print_float"), expr_create(EXPR_ARG, curr->left, NULL, 8), 8);
                        expr_codegen_fcall_float(e);
                        scratch_free(e->reg);
                        break;
                    default:
                        printf("codegen error: an unprintable type %d was found in a print statement.\n", t->kind);
                        exit(1);
                        break;
                }
            }
            break;
        case STMT_BR:
            stmt_codegen(s->body, function_name);
            break;

    }
    stmt_codegen(s->next, function_name);
}