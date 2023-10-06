#include "bminor_helper.h"

int token_to_string (token_t token, char * yytext){
    // tokens
    switch (token) {
        // keywords
        case TOKEN_ARRAY:
            printf("ARRAY %s\n", yytext);
            break;
        case TOKEN_AUTO:
            printf("AUTO %s\n", yytext);
            break;
        case TOKEN_BOOLEAN:
            printf("BOOLEAN %s\n", yytext);
            break;
        case TOKEN_CHAR:
            printf("CHAR %s\n", yytext);
            break;
        case TOKEN_ELSE:
            printf("ELSE %s\n", yytext);
            break;
        case TOKEN_FALSE:
            printf("FALSE %s\n", yytext);
            break;
        case TOKEN_FOR:
            printf("FOR %s\n", yytext);
            break;
        case TOKEN_FUNCTION:
            printf("FUNCTION %s\n", yytext);
            break;
        case TOKEN_IF:
            printf("IF %s\n", yytext);
            break;
        case TOKEN_INT:
            printf("INTEGER %s\n", yytext);
            break;
        case TOKEN_PRINT:
            printf("PRINT %s\n", yytext);
            break;
        case TOKEN_RETURN:
            printf("RETURN %s\n", yytext);
            break;
        case TOKEN_STRING:
            printf("STRING %s\n", yytext);
            break;
        case TOKEN_TRUE:
            printf("TRUE %s\n", yytext);
            break;
        case TOKEN_VOID:
            printf("VOID %s\n", yytext);
            break;
        case TOKEN_WHILE:
            printf("WHILE %s\n", yytext);
            break;
        // expression symbol:
        case TOKEN_LPAREN:
            printf("PARENTHESIS_OPEN %s\n", yytext);
            break;
        case TOKEN_RPAREN:
            printf("PARENTHESIS_CLOSE %s\n", yytext);
            break;
        case TOKEN_LBRACKET:
            printf("BRACKET_OPEN %s\n", yytext);
            break;
        case TOKEN_RBRACKET:
            printf("BRACKET_OPEN %s\n", yytext);
            break;
        case TOKEN_LBRACE:
            printf("BRACE_OPEN %s\n", yytext);
            break;
        case TOKEN_RBRACE:
            printf("BRACE_CLOSE %s\n", yytext);
            break;
        case TOKEN_INCREMENT:
            printf("INCREMENT %s\n", yytext);
            break;
        case TOKEN_DECREMENT:
            printf("DECREMENT %s\n", yytext);
            break;
        case TOKEN_NOT:
            printf("NOT %s\n", yytext);
            break;
        case TOKEN_EXP:
            printf("EXPONENTIATION %s\n", yytext);
            break;
        case TOKEN_MULT:
            printf("MULTIPLY %s\n", yytext);
            break;
        case TOKEN_DIV:
            printf("DIVIDE %s\n", yytext);
            break;
        case TOKEN_MOD:
            printf("MODULUS %s\n", yytext);
            break;
        case TOKEN_PLUS:
            printf("PLUS %s\n", yytext);
            break;
        case TOKEN_MINUS:
            printf("SUBTRACT %s\n", yytext);
            break;
        case TOKEN_GT:
            printf("GT %s\n", yytext);
            break;
        case TOKEN_LT:
            printf("LT %s\n", yytext);
            break;
        case TOKEN_GTE:
            printf("GTE %s\n", yytext);
            break;
        case TOKEN_LTE:
            printf("LTE %s\n", yytext);
            break;
        case TOKEN_EQ:
            printf("EQ %s\n", yytext);
            break;
        case TOKEN_NOT_EQ:
            printf("NOT_EQ %s\n", yytext);
            break;
        case TOKEN_ASSIGN:
            printf("ASSIGN %s\n", yytext);
            break;
        case TOKEN_COMMA:
            printf("COMMA %s\n", yytext);
            break;
        case TOKEN_SEMICOL:
            printf("SEMICOLON %s\n", yytext);
            break;
        case TOKEN_COLON:
            printf("COLON %s\n", yytext);
            break;
        case TOKEN_AND:
            printf("AND %s\n", yytext);
            break;
        case TOKEN_OR:
            printf("OR %s\n", yytext);
            break;
        // identifier
        case TOKEN_IDENT:
            printf("IDENTIFIER %s\n", yytext);
            break;
        // literals
        case TOKEN_STR_LIT:
            printf("STRING_LITERAL %s\n", yytext);
            break;
        case TOKEN_CHAR_LIT:
            printf("CHARACTER_LITERAL %s\n", yytext);
            break;
        case TOKEN_INT_LIT:
            printf("INTEGER_LITERAL %s\n", yytext);
            break;
        case TOKEN_FLOAT_LIT:
            printf("FLOAT_LITERAL %s\n", yytext);
            break;
        case TOKEN_ERROR:
            fprintf(stdout, "scan error: %s is not a valid character\n", yytext);
            return 1;

        default:
            fprintf(stdout, "scan error: %s is not a valid character\n", yytext);
            return 1;
    }
    return 0;
}

int scan(){
    while(1){
        int t = yylex();
        if (t==0) return 0;
        if (token_to_string(t, yytext) == 1) return 1;
    }
    return 0;
}

int encode(){
    char input[BUFSIZ];
    char decoded[BUFSIZ];
    char output[BUFSIZ];
    
    if(fgets(input, sizeof(input), yyin) != NULL){
        /* decode the code */
        if (input[0] == '\0') {
            printf("Empty Input\n");
        }
        if (input[strlen(input)-2] == '\n') {
            input[strlen(input)] = '\0';
        }
        printf("Input String    : %s", input);
        printf("\n");
        int result = string_decode(input, decoded);
        if (result == 1) {
            printf("Decoded String    : %s", decoded);
            printf("\n");
            string_encode(decoded, output);
            printf("Output String    : %s", output);
            printf("\n");
        } else {
            printf("Invalid String: %s\n", input);
            return EXIT_FAILURE;
        }
    } else {
        printf("Null file descriptor\n");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
 