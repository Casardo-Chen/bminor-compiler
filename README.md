B-Minor Compiler
-------------------------------
This code repo contains the code for B-Minor compiler.

B-Minor includes expressions, basic control flow, recursive functions, and strict type checking. It is object-code compatible with ordinary C and thus can take advantage of the standard C library, within its defined types.

An overview of B-Minor language can be found here: [2023 BMinor Language](https://dthain.github.io/compilers-fa23/bminor)
```
./
├── bin                             
│   ├── test_codegen_unit.sh
│   ├── test_encoder_unit.sh
│   ├── test_parser_unit.sh
│   ├── test_printer_unit.sh
│   ├── test_resolver_unit.sh
│   ├── test_scanner_unit.sh
│   └── test_typechecker_unit.sh
├── include                     
│   ├── bminor_helper.h
│   ├── decl.h
│   ├── encoder.h
│   ├── expr.h
│   ├── hash_table.h
│   ├── param_list.h
│   ├── scope.h
│   ├── scratch.h
│   ├── stmt.h
│   ├── symbol.h
│   └── type.h
├── Makefile
├── README.md
├── runtest.sh
├── src
│   ├── bminor_helper.c             
│   ├── decl.c
│   ├── encoder.c
│   ├── expr.c
│   ├── hash_table.c                
│   ├── library.c
│   ├── main.c
│   ├── param_list.c
│   ├── parser.y
│   ├── scanner.l
│   ├── scope.c
│   ├── scratch.c
│   ├── stmt.c
│   ├── symbol.c
│   └── type.c                      
└── test
```
### How to Run the Code
1. Enter the main directory and run `make`.
2. After successfully compiling `bminor`, you may run the code!

    For encode, scan, parse, print, resolve, and typecheck:
    ```
    ./bin/bminor [--encode|--scan|--parse|--print|--resolve|--typecheck] input.bminor
    ```
    For codegen:
    ```
    ./bin/bminor --codegen input.bminor output.s
    gcc -g ouput.s library.c -o program
    ```
3. You can run `make test` to test the functionality of the code. You may also each step in compiling respectively by running `make test-[encode|scan|parse|print|resolve|typecheck|codegen]`

4. You can run `make clean` to remove all the outputs in the `test` folder and the `.o` files in `bin` folder.

### What is in This Compiler
There are 6 steps in B-Minor Compiler:
- **Encoder:** encoding and decoding of strings
- **Scanner:** applying the Flex Scanner;Generator to scan the tokens in a `.bminor` file via regular expression matching;
- **Parser:** applying the Bison Parser Generator to parse the tokens using Context Free Grammar;
- **Printer:** pretty printing out the same `.bminor` based on the constructed Abstract Synax Tree (AST);
- **Resolver & Typechecker:** traversing the AST and checking the semantic correctness of the program, particularly focusing on name resolution and type checking;
- **Codegen:** reading in a B-minor program and emit an 64-bit X86 assembly language program that can be assembled, linked, and run.

### TODOs:
- Implement support for floating point values. It should be possible to declare and use floating point values at global scope, local scope, in function paramers, in arrays, in expressions, and in print statements.
- Implement support for dynamically allocated and checked arrays.


