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
            if (!s->pre_indent) indent_print(indent);
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

