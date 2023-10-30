#include "expr.h"
#include "encoder.h"
#include "library.c"

struct expr * expr_create( expr_t kind, struct expr *left, struct expr *right ){
    struct expr *e = (struct expr * )malloc(sizeof(struct expr));
    if(!e){
        printf("print error: failed to allocate memory for expr.\n");
        exit(1);
    }

    e->kind = kind;
    e->left = left;
    e->right = right;
    e->name = NULL;
    e->literal_value = 0;
    e->string_literal = NULL;
    e->symbol = NULL;

    return e;
}

struct expr * expr_create_name( const char *n ){
    struct expr *e = (struct expr * ) malloc(sizeof(struct expr));
    if(!e){
        printf("print error: failed to allocate memory for expr.\n");
        exit(1);
    }
    e->kind = EXPR_IDENT;
    e->left = NULL;
    e->right = NULL;
    e->name = strdup(n);
    e->literal_value = 0;
    e->string_literal = NULL;
    e->symbol = NULL;

    return e;
}

struct expr * expr_create_integer_literal( int c ){
    struct expr *e = (struct expr * ) malloc(sizeof(struct expr));
    if(!e){
        printf("print error: failed to allocate memory for expr.\n");
        exit(1);
    }
    e->kind = EXPR_INT_LIT;
    e->left = NULL;
    e->right = NULL;
    e->name = NULL;
    e->literal_value = c;
    e->string_literal = NULL;
    e->symbol = NULL;
    
    return e;
}

struct expr * expr_create_boolean_literal( int c ){
    struct expr *e = (struct expr * ) malloc(sizeof(struct expr));
    if(!e){
        printf("print error: failed to allocate memory for expr.\n");
        exit(1);
    }
    e->kind = EXPR_BOOL_LIT;
    e->left = NULL;
    e->right = NULL;
    e->name = NULL;
    e->literal_value = c;
    e->string_literal = NULL;
    e->symbol = NULL;
    
    return e;
}

struct expr * expr_create_float_literal( const char *f ){
    struct expr *e = (struct expr * ) malloc(sizeof(struct expr));
    if(!e){
        printf("print error: failed to allocate memory for expr.\n");
        exit(1);
    }
    e->kind = EXPR_FLOAT_LIT;
    e->left = NULL;
    e->right = NULL;
    e->name = NULL;
    e->literal_value = 0;
    e->string_literal = strdup(f);
    e->symbol = NULL;
    
    return e;
}

struct expr * expr_create_char_literal( char c ){
    struct expr *e = (struct expr * ) malloc(sizeof(struct expr));
    if(!e){
        printf("print error: failed to allocate memory for expr.\n");
        exit(1);
    }
    e->kind = EXPR_CHAR_LIT;
    e->left = NULL;
    e->right = NULL;
    e->name = NULL;
    e->literal_value = (int) c;
    e->string_literal = NULL;
    e->symbol = NULL;
    
    return e;

}

struct expr * expr_create_string_literal( const char *str ){
    struct expr *e = (struct expr * ) malloc(sizeof(struct expr));
    if(!e){
        printf("print error: failed to allocate memory for expr.\n");
        exit(1);
    }
    e->kind = EXPR_STR_LIT;
    e->left = NULL;
    e->right = NULL;
    e->name = NULL;
    e->literal_value = 0;
    e->string_literal = strdup(str);
    e->symbol = NULL;
    
    return e;

}

void expr_print( struct expr *e ){
    if(!e) return;
    char es[BUFSIZ];
    switch (e->kind){
    case EXPR_ADD:
        print_op(e, "+");
        break;
    case EXPR_AND:
        print_op(e, "&&");
        break;
    case EXPR_ASSIGN:
        print_op(e, "=");
        break;
    case EXPR_DIV:
        print_op(e, "/");
        break;
    case EXPR_DECREMENT:
        expr_print(e->left);
        printf("--");
        break;
    case EXPR_EQ:
        print_op(e, "==");
        break;
    case EXPR_EXP:
        print_op(e, "^");
        break;
    case EXPR_GT:
        print_op(e, ">");
        break;
    case EXPR_GTE:
        print_op(e, ">=");
        break;
    case EXPR_INCREMENT:
        expr_print(e->left);
        printf("++");
        break;
    case EXPR_LT:
        print_op(e, "<");
        break;
    case EXPR_LTE:
        print_op(e, "<=");
        break;
    case EXPR_MOD:
        print_op(e, "\%");
        break;
    case EXPR_MUL:
        print_op(e, "*");
        break;
    case EXPR_NEG:
        printf("-");
        expr_print(e->left);
        break;
    case EXPR_NOT:
        printf("!");
        expr_print(e->left);
        break;
    case EXPR_NOT_EQ:
        print_op(e, "!=");
        break;
    case EXPR_OR:
        print_op(e, "||");
        break;
    case EXPR_POS:
        printf("+");
        expr_print(e->left);
        break;
    case EXPR_PAREN:
        if (e->left->kind == EXPR_PAREN) {
            expr_print(e->left);
            break;
        };
        printf("( ");
        expr_print(e->left);
        printf(" )");
        break;
    case EXPR_SUB:
        print_op(e, "-");
        break;
    
    case EXPR_IDENT:
        printf("%s", e->name);
        break;
    case EXPR_INT_LIT:
        print_integer((long) e->literal_value);
        break;
    case EXPR_BOOL_LIT:
        print_boolean(e->literal_value);
        break;
    case EXPR_CHAR_LIT:
        printf("'");
        print_character((char)e->literal_value);
        printf("'");
        break;
    case EXPR_FLOAT_LIT:
        printf("%g", atof(e->string_literal));
        break;
    case EXPR_STR_LIT:
        string_encode(e->string_literal, es);
        print_string(es);
        break;
    
    case EXPR_SUBT:
        expr_print(e->left);
        printf("[");
        expr_print(e->right);
        printf("]");
        break;    
    case EXPR_CALL:
        expr_print(e->left);
        printf("(");
        expr_print(e->right);
        printf(")");
        break;
    case EXPR_ARG:
        expr_print(e->left);
        if (!e->right) break;
        printf(", ");
        expr_print(e->right);
        break;
    case EXPR_BR:
        printf("{");
        expr_print(e->left);
        printf("}");
        if (e->right) printf(",");
        expr_print(e->right);
        break;
    default:
        printf("print error: invalid expression type found.\n");
        exit(1);
        break;
    }
}

void print_op(struct expr *e, const char * op ){
    expr_print(e->left);
    printf("%s", op);
    expr_print(e->right);
}

