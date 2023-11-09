#include "expr.h"
#include "encoder.h"
#include "library.c"

struct expr * expr_create( expr_t kind, struct expr *left, struct expr *right, int precedence ){
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
    e->precedence = precedence;

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
    e->precedence = 8;

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
    e->precedence = 8;
    
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
    e->precedence = 8;
    
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
    e->precedence = 8;
    
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
    e->precedence = 8;
    
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
    e->precedence = 8;
    
    return e;

}

void expr_print( struct expr *e ){
    if(!e) return;

    // handling char and string
    char es[BUFSIZ];
    char s[2] = "\0";
    char ec[BUFSIZ];

    switch (e->kind){
    case EXPR_ADD:
        e = expr_check(e,LEFT);
        print_op(e, "+");
        break;
    case EXPR_AND:
        e = expr_check(e,LEFT);
        print_op(e, "&&");
        break;
    case EXPR_ASSIGN:
        e = expr_check(e,RIGHT);
        print_op(e, "=");
        break;
    case EXPR_DIV:
        e = expr_check(e,LEFT);
        print_op(e, "/");
        break;
    case EXPR_DECREMENT:
        e = expr_check(e,LEFT);
        expr_print(e->left);
        printf("--");
        break;
    case EXPR_EQ:
        e = expr_check(e,LEFT);
        print_op(e, "==");
        break;
    case EXPR_EXP:
        e = expr_check(e,LEFT);
        print_op(e, "^");
        break;
    case EXPR_GT:
        e = expr_check(e,LEFT);
        print_op(e, ">");
        break;
    case EXPR_GTE:
        e = expr_check(e,LEFT);
        print_op(e, ">=");
        break;
    case EXPR_INCREMENT:
        e = expr_check(e,LEFT);
        expr_print(e->left);
        printf("++");
        break;
    case EXPR_LT:
        e = expr_check(e,LEFT);
        print_op(e, "<");
        break;
    case EXPR_LTE:
        e = expr_check(e,LEFT);
        print_op(e, "<=");
        break;
    case EXPR_MOD:
        e = expr_check(e,LEFT);
        print_op(e, "\%");
        break;
    case EXPR_MUL:
        e = expr_check(e,LEFT);
        print_op(e, "*");
        break;
    case EXPR_NEG:
        printf("-");
        expr_print(e->left);
        break;
    case EXPR_NOT:
        e = expr_check(e,NONE);
        printf("!");
        expr_print(e->left);
        break;
    case EXPR_NOT_EQ:
        e = expr_check(e,LEFT);
        print_op(e, "!=");
        break;
    case EXPR_OR:
        e = expr_check(e,LEFT);
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
        printf("(");
        expr_print(e->left);
        printf(")");
        break;
    case EXPR_SUB:
        e = expr_check(e,LEFT);
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
		s[0] = (char)e->literal_value;
		string_encode(s, es);
		sscanf(es, "\"%[^\"]", ec);
		printf("\'%s\'", ec);
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

struct expr * expr_check( struct expr *e, assoc_t assoc) {
    // check if the paren is necessary
    // precedence represents the level of the node in the AST tree
    if (e->left && e->left->precedence < e->precedence) {
        e->left = expr_wrap(e->left);
    }
    if (e->right && e->right->precedence < e->precedence) {
        e->right = expr_wrap(e->right);
    }
    if (assoc == LEFT && e->right && e->right->precedence == e->precedence) {
        e->right = expr_wrap(e->right);
    }
    if (assoc == RIGHT && e->left && e->left->precedence == e->precedence) {
        e->left = expr_wrap(e->left);
    }
    return e;
}

struct expr * expr_wrap( struct expr * e ){
    return expr_create(EXPR_PAREN, e, 0, 8);
}

void expr_resolve( struct expr *e ){
    if (!e) return;
    if (e->kind == EXPR_IDENT){
        e->symbol = scope_lookup(e->name);
        if (e->symbol){
            switch (e->symbol->kind)
            {
                case SYMBOL_GLOBAL:
                    printf("%s resolves to global %s\n", e->symbol->name, e->symbol->name);
                    break;
                case SYMBOL_LOCAL:
                    printf("%s resolves to local %d\n", e->symbol->name, e->symbol->which);
                    break;
                case SYMBOL_PARAM:
                    printf("%s resolves to param %d\n", e->symbol->name, e->symbol->which);
                    break;
                default:
                    printf("resolve error: undefined symbol kind.\n");
                    resolve_error++;
                    break;
            }
        } else {
            printf("resolve error: %s is not defined.\n", e->name);
            resolve_error++;
        }
    } else {
        expr_resolve( e->left );
        expr_resolve( e->right );
    }
    
    return;
}
