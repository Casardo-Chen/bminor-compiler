#include "./encoder.h"

// Function to decode an encoded string containing quotes and backslash codes
bool string_decode(const char *es, char *s) {
    int i = 0, j = 0, quote = 0;
    int len = strlen(es);
    // check if starts with a double quote
    if (len == 0 || es[0] != '\"') {
        printf("Error: empty/not starting with \"\n");
        return false;
    }
    // printf("es %d : %c\n", i, es[i]);
    i++;
    quote++;

    while (i < len) {
        // Check for a backslash code
        // printf("es %d : %c\n", i, es[i]);
        if (es[i] == '\\') {
            i++; // Skip the backslash
            if (i >= len) {
                printf("Error: exceed the len\n");
                return false; // Backslash at the end of the string is invalid
            }
            // printf("es %d : %c\n", i, es[i]);
            // Handle escaped characters
            switch (es[i]) {
                case 'a':
                    s[j] = '\a';
                    break;
                case 'b':
                    s[j] = '\b';
                    break;
                case 'e':
                    s[j] = '\e';
                    break;
                case 'f':
                    s[j] = '\f';
                    break;
                case 'n':
                    s[j] = '\n';
                    break;
                case 'r':
                    s[j] = '\r';
                    break;
                case 't':
                    s[j] = '\t';
                    break;
                case 'v':
                    s[j] = '\v';
                    break;
                case '\\':
                    s[j] = '\\';
                    break;
                case '\'':
                    s[j] = '\'';
                    break;
                case '\"':
                    s[j] = '\"';
                    break;
                case '0': // 0xHH case
                    i++;
                    if (i >= len) {
                        printf("Error: exceed the len\n");
                        return false;
                    }
                    // printf("es %d : %c\n", i, es[i]);
                    if (es[i] == 'x'){
                        // Store the two hexadecimal character
                        char hex[3]; 
                        i++;
                        if (i >= len) {
                            printf("Error: exceed the len\n");
                            return false;
                        }
                        hex[0] = es[i]; 
                        // printf("es %d : %c\n", i, es[i]);
                        i++;
                        if (i >= len) {
                            printf("Error: exceed the len\n");
                            return false;
                        }
                        // printf("es %d : %c\n", i, es[i]);
                        hex[1] = es[i];
                        hex[2] = '\0';
                        // Convert the hexadecimal string to an integer
                        int value;
                        if (sscanf(hex, "%x", &value) != 1) {
                            return false; // Invalid hexadecimal escape sequence
                        }
                        // printf("value %d\n", (unsigned char)value);
                        if ((unsigned char)value==0 && quote) {
                            printf("Error: \\0x00 appear before the string is closed\n");
                            return false; // Invalid hexadecimal escape sequence
                        }
                        s[j] = (unsigned char)value;
                        break;
                    } else {
                        printf("Error: Invalid 0xHH\n");
                        return false;
                    }
                default:
                    printf("Error: Invalid escape sequence\n");
                    return false; // Invalid escape sequence
            }
        } else if (es[i] == '\"') { 
            /* if it is double quote, then check if it is:
                the end:            invalid since it appears without an escape backslash
                in the middle:      end of a new string
            */
            i++;
            if (es[i] == '\0') {
                // Properly terminated string
                // |l-1 |l-2    |l-1    | 
                // |"   |\      |0      |
                s[j] = '\0'; // Null-terminate the decoded string
                // printf("Success: get the decoded string\n");
                return true;
            } else {
                // Unescaped quote in the middle is invalid
                printf("Error: Unescaped quote in the middle is invalid\n");
                return false;
            }
        } else {
            s[j] = es[i];
        }
        i++;
        j++;
    }
    return false;
}

// // Function to encode a normal string into an encoded string with quotes and backslash codes
void string_encode(const char *s, char *es) {
    int i = 0, j = 0;
    es[j++] = '\"';
    
    while (s[i] != '\0') {
        switch (s[i]) {
            // if the string has a ascii num, convert it into '\' + 'character'
            case '\a':
                es[j++] = '\\';
                es[j] = 'a';
                break;
            case '\b':
                es[j++] = '\\';
                es[j] = 'b';
                break;
            case '\e':
                es[j++] = '\\';
                es[j] = 'e';
                break;
            case '\f':
                es[j++] = '\\';
                es[j] = 'f';
                break;
            case '\n':
                es[j++] = '\\';
                es[j] = 'n';
                break;
            case '\r':
                es[j++] = '\\';
                es[j] = 'r';
                break;
            case '\t':
                es[j++] = '\\';
                es[j] = 't';
                break;
            case '\v':
                es[j++] = '\\';
                es[j] = 'v';
                break;
            case '\\':
                es[j++] = '\\';
                es[j] = '\\';
                break;
            case '\'':
                es[j++] = '\\';
                es[j] = '\'';
                break;
            case '\"':
                es[j++] = '\\';
                es[j] = '\"';
                break;
            default:
                if (s[i] >= 0x20 && s[i] <= 0x7E) {
                    // Printable ASCII characters
                    es[j] = s[i];
                } 
                else {
                    // Non-printable characters as hexadecimal escape sequence
                    sprintf(&es[j], "\\0x%02X", (unsigned char)s[i]);
                    j += 4;
                }
        }
        i++;
        j++;
    }
    es[j++] = '\"';
    es[j] = '\0'; // Null-terminate the encoded string
    return;
}