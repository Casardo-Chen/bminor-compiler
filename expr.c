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

struct expr* expr_copy(struct expr *e){
    if(!e) return NULL;
    return expr_create( e->kind, expr_copy(e->left), expr_copy(e->right), e->precedence );
}

void expr_delete( struct expr *e){
    if (!e) return;
    expr_delete(e->left);
    expr_delete(e->right);
    free(e);
}

struct type* expr_typecheck(struct expr* e){
    if(!e) return NULL;
    struct type * lt = expr_typecheck(e->left);
    struct type * rt = expr_typecheck(e->right);
    struct type * t;
    switch (e->kind){
        case EXPR_ADD:
        case EXPR_SUB:
        case EXPR_MUL:
        case EXPR_MOD:
        case EXPR_DIV:
        case EXPR_EXP:
            // integer
            if (type_eq(lt,rt)){
                if(lt->kind == TYPE_INTEGER){  
                    t = type_create(TYPE_INTEGER, 0, 0, 0);
                } else if (lt->kind == TYPE_FLOAT){
                    t = type_create(TYPE_FLOAT, 0, 0, 0);
                } else {
                    expr_error_print(e, lt, rt);
                    t = type_create(TYPE_ERROR, 0, 0, 0);
                }
            } else {
                expr_error_print(e, lt, rt); 
                t = type_create(TYPE_ERROR, 0, 0, 0);
            } 
            break;
        case EXPR_BOOL_LIT:
            t = type_create(TYPE_BOOLEAN, 0, 0, 0);
            break;
        case EXPR_INT_LIT:
            t = type_create(TYPE_INTEGER, 0, 0, 0);
            break;
        case EXPR_STR_LIT:
            t = type_create(TYPE_STRING, 0, 0, 0);
            break;
        case EXPR_FLOAT_LIT:
            t = type_create(TYPE_FLOAT, 0, 0, 0);
            break;
        case EXPR_CHAR_LIT:
            t = type_create(TYPE_CHARACTER, 0, 0, 0);
            break;
        case EXPR_INCREMENT:
        case EXPR_DECREMENT:
            if (e->left->kind != EXPR_IDENT && e->left->kind != EXPR_SUBT) {
                printf("type error: can only apply increment/decrement on variables or array elements.\n");
            }
            if (lt->kind != TYPE_INTEGER) {
                expr_error_print(e, lt, rt);
            }
            t = type_create(TYPE_INTEGER, 0, 0, 0);
            break;
        case EXPR_AND:
        case EXPR_OR:
            if (lt->kind != TYPE_BOOLEAN || rt->kind != TYPE_BOOLEAN) {
                expr_error_print(e, lt, rt);
            }
            t = type_create(TYPE_BOOLEAN, 0, 0, 0);
            break;
        case EXPR_NEG:
            if (lt->kind != TYPE_INTEGER) {
                expr_error_print(e, lt, NULL);
                t = type_create(TYPE_INTEGER, 0, 0, 0);
            } else if (lt->kind != TYPE_FLOAT){
                expr_error_print(e, lt, NULL);
                t = type_create(TYPE_FLOAT, 0, 0, 0);
            }
            break;
        case EXPR_NOT:
            if (lt->kind != TYPE_BOOLEAN) {
                expr_error_print(e, lt, NULL);
            }
            t = type_create(TYPE_BOOLEAN, 0, 0, 0);
            break;
        case EXPR_IDENT:
            t = type_copy(e->symbol->type);
            break;
        case EXPR_LT:
        case EXPR_LTE:
        case EXPR_GT:
        case EXPR_GTE:
            if (type_eq(lt,rt)){
                if(lt->kind != TYPE_INTEGER && lt->kind != TYPE_FLOAT){  
                    expr_error_print(e, lt, rt); 
                } 
            } else {
                expr_error_print(e, lt, rt); 
            }
            t = type_create(TYPE_BOOLEAN, 0, 0, 0);
            break;
        case EXPR_EQ:
        case EXPR_NOT_EQ:
            if (!type_eq(lt, rt)) {
                expr_error_print(e, lt, rt);
            } else if (lt->kind ==TYPE_VOID || lt->kind ==TYPE_ARRAY || lt->kind == TYPE_FUNCTION) {
                expr_error_print(e, lt, rt);
            }
            t = type_create(TYPE_BOOLEAN, 0, 0, 0);
            break;
        case EXPR_CALL:
            // E.g. writechar(a,b)
            if (lt->kind == TYPE_FUNCTION){
                struct expr * r = e->right;         // a,b...
                struct param_list * p = lt->params; // definition of the function
                while (r || p) {
                    if ((!r && p) || (r && !p)) {
                        printf("type error: number of arguments for function call %s doesn't match the number of parameters declared in definition.\n", e->left->name);
                        type_error++;
                        break;
                    }
                    struct type * a = expr_typecheck(r->left); // type of param
                    if (a->kind == TYPE_FUNCTION || a->kind == TYPE_VOID) {
                        printf("type error: function argument cannot be ");
                        type_print(a);
                        printf(" (");
                        expr_print(r->left);
                        printf(")\n");
                        type_error++;
                    }
                    if (!type_eq(p->type, a)) {
                        printf("type error: found type ");
                        type_print(a);
                        printf(" (");
                        expr_print(r->left);
                        printf(") in function %s ", e->left->name);
                        printf(", which should be ");
                        type_print(p->type);
                        printf("\n");
                        type_error++;
                    }
                    // type_delete(a);
                    p = p->next;
                    r = r->right;
                }
                t = type_copy(lt->subtype);
            } else {
                // invalid call
                printf("type error: attempted to call ");
                type_print(lt);
                printf(" (");
                expr_print(e->left);
                printf("), which is not a function\n");
                type_error++;
                t = type_copy(lt);
            }
            break;
        case EXPR_ARG: //a,b,c,d
            t = type_copy(lt);
            break;
        case EXPR_SUBT: // a[i]
            if (lt->kind == TYPE_ARRAY) {
                if (rt->kind != TYPE_INTEGER) {
                    printf("type error: index is a ");
                    type_print(rt);
                    printf(" (");
                    expr_print(e->right);
                    printf(") it must be an integer\n");
                    type_error++;
                }
                t = type_copy(lt->subtype);
            } else {
                // not an array
                expr_error_print(e, lt, NULL);
                // return valid type anyways
                t = type_copy(lt);
            }
            break;
        case EXPR_BR: // {...}
            if (e->left->kind == EXPR_ARG) {
                int counter = 0;
                struct expr *curr = e->left; // current node
                struct type *curr_type = NULL;
                while (curr) {
                    struct type *a = expr_typecheck(e->left);
                    if (!curr_type) curr_type = a;
                    // type inside an array should be consistent
                    if (!type_eq(curr_type, a)) {
                        printf("type error: inconsistent types in the array: ");
                        type_print(a);
                        printf(" and ");
                        type_print(curr_type);
                        printf("\n");
                        type_error++;
                    }
                    // array value cannot be function or void
                    if (a->kind == TYPE_VOID || a->kind == TYPE_FUNCTION) {
                        printf("type error: array values cannot be ");
                        type_print(a);
                        printf(" (");
                        expr_print(curr->left);
                        printf(")\n");
                        type_error++;
                    }
                    // if (counter > 0) type_delete(a);
                    counter++;
                    curr = curr->right;
                }
                t = type_create(TYPE_ARRAY, curr_type, NULL, expr_create_integer_literal(counter));
                // {},{},{}
                if (e->right && !e->literal_value) {
                    struct expr* curr = e->right;
                    counter = 1;
                    while (curr) {
                        curr->literal_value = 1;
                        struct type* right = expr_typecheck(curr);
                        if (!type_eq(t, right)) {
                            printf("type error: array values (");
                            type_print(t);
                            printf(") and (");
                            type_print(right);
                            printf(") are inconsistent\n");
                            type_error++;
                        }
                        // type_delete(right);
                        curr = curr->right;
                        counter += 1;
                    }
                    t = type_create(TYPE_ARRAY, t, NULL, expr_create_integer_literal(counter));
                }
            //f it is already an expr brace
            } else if (e->left->kind == EXPR_BR) {
                t = type_copy(lt);
                if (e->right && !e->literal_value) {
                    struct expr* curr = e->right;
                    int counter = 1;
                    while (curr) {
                        curr->literal_value = 1;
                        struct type* right = expr_typecheck(curr);
                        if (!type_eq(t, right)) {
                            printf("type error: array values (");
                            type_print(t);
                            printf(") and (");
                            type_print(right);
                            printf(") are inconsistent\n");
                            type_error++;
                        }
                        // type_delete(right);
                        curr = curr->right;
                        counter += 1;
                    }
                    t = type_create(TYPE_ARRAY, t, 0, expr_create_integer_literal(counter));
                }  
            }
            break;
        case EXPR_ASSIGN:
            if (e->left->kind != EXPR_IDENT && e->left->kind != EXPR_SUBT) {
                printf("type error: can only apply assignment on variables or array elements.\n");
                type_error++;
            }
            if (!type_eq(lt, rt)) {
                expr_error_print(e, lt, rt);
                type_error++;
            }
            if (rt->kind==TYPE_FUNCTION || rt->kind == TYPE_VOID) {
                expr_error_print(e, lt, rt);
                type_error++;
            }
            t = type_copy(rt);
            break;
        case EXPR_PAREN:
            t = type_copy(rt);
        default:
            break;
    }
    
    // type_delete(lt);
    // type_delete(rt);
    return t;
}

 void expr_error_print(struct expr* e, struct type* lt, struct type* rt){
        char * op;
        switch (e->kind) {
            /* ops */
        case EXPR_ADD:
            op = "add";
            break;
        case EXPR_SUB:
            op = "subtract";
            break;
        case EXPR_MUL:
            op = "multiply";
            break;
        case EXPR_DIV:
            op = "divide";
            break;
        case EXPR_MOD:
            op = "compute modulus of";
            break;
        case EXPR_EXP:
            op = "exponentiate";
            break;
        case EXPR_INCREMENT:
            op = "increment";
            break;
        case EXPR_DECREMENT:
            op = "decrement";
            break;
        case EXPR_SUBT:
            op = "index";
            break;
        case EXPR_ARG:
            op = "arg";
            break;
        case EXPR_NEG:
            op = "negate";
            break;
        case EXPR_NOT:
            op = "logical invert";
            break;
        case EXPR_GT:
        case EXPR_GTE:
        case EXPR_LT:
        case EXPR_LTE:
        case EXPR_EQ:
        case EXPR_NOT_EQ:
            op = "compare";
            break;
        case EXPR_AND:
            op = "logical and";
            break;
        case EXPR_OR:
            op = "logical or";
            break;
        case EXPR_ASSIGN:
            op = "assign";
            break;
        case EXPR_IDENT:
            op = "id";
            break;
        case EXPR_INT_LIT:
            op = "integer literal";
            break;
        case EXPR_BOOL_LIT:
            op = "boolean literal";
            break;
        case EXPR_CHAR_LIT:
            op = "char literal";
            break;
        case EXPR_STR_LIT:
            op ="string literal";
            break;
        default:
            printf("type error: invalid expression type found.\n");
            type_error++;
    }
    
    if (rt) {
        printf("type error: cannot %s ", op);
        type_print(rt);
        printf(" (");
        expr_print(e->right);
        printf(") to ");
        type_print(lt);
        printf(" (");
        expr_print(e->left);
        printf(")\n");
    } else {
        printf("type error: cannot %s ", op);
        type_print(lt);
        printf(" (");
        expr_print(e->left);
        printf(")\n");
    }
    type_error++;
 }

 int expr_const(struct expr *e){
    if (!e) return 1;
    switch (e->kind){
        case EXPR_CHAR_LIT:
        case EXPR_INT_LIT:
        case EXPR_STR_LIT:
        case EXPR_FLOAT_LIT:
        case EXPR_BOOL_LIT:
            return 1;
        case EXPR_ARG:
        case EXPR_BR:
            return expr_const(e->left) && expr_const(e->right);
        default:
            return 0;
    }
}
