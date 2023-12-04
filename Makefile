# @ Author: Meng Chen
# @ Date: 10/10/2023

###############
# c compilers #
###############
CC = gcc 
CFLAGS = -Wall -I$(INCLUDE_DIR)

#########
# files #
#########
ENCODER = encoder
SCANNER = scanner
PARSER = parser
TOKEN = token
MAIN = main
HELPER = bminor_helper
EXEC = bminor
TEST = runtest.sh

EXPR = expr
STMT = stmt
PARAM = param_list
DECL = decl
TYPE = type

SYM = symbol
SCOPE = scope
HT = hash_table
SCRATCH = scratch

###############
# Directories #
###############
TEST_DIR = test
BIN_DIR = bin
SRC_DIR = src
INCLUDE_DIR = include


###############
# 	  make    #
###############
OBJECTS = $(BIN_DIR)/$(MAIN).o $(BIN_DIR)/$(HELPER).o $(BIN_DIR)/$(ENCODER).o $(BIN_DIR)/$(SCANNER).o $(BIN_DIR)/$(PARSER).o $(BIN_DIR)/$(EXPR).o $(BIN_DIR)/$(STMT).o $(BIN_DIR)/$(PARAM).o $(BIN_DIR)/$(DECL).o $(BIN_DIR)/$(TYPE).o $(BIN_DIR)/$(SYM).o $(BIN_DIR)/$(SCOPE).o $(BIN_DIR)/$(HT).o $(BIN_DIR)/$(SCRATCH).o

$(BIN_DIR)/$(EXEC): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ 

$(BIN_DIR)/$(MAIN).o: $(SRC_DIR)/$(MAIN).c $(INCLUDE_DIR)/$(TOKEN).h
	$(CC) $(CFLAGS) -c $< -o $@

# bin
$(BIN_DIR)/%.o: $(SRC_DIR)/%.c $(INCLUDE_DIR)/*.h
	$(CC) $(CFLAGS) -c $<  -o $@

# scanner
$(SRC_DIR)/$(SCANNER).c: $(SRC_DIR)/$(SCANNER).l
	flex -o $@ $^

$(BIN_DIR)/$(SCANNER).o: $(SRC_DIR)/$(SCANNER).c $(INCLUDE_DIR)/$(TOKEN).h
	$(CC) -I$(INCLUDE_DIR) -c $<  -o $@

# parser
$(SRC_DIR)/$(PARSER).c $(INCLUDE_DIR)/$(TOKEN).h: $(SRC_DIR)/$(PARSER).y
	bison --defines=$(INCLUDE_DIR)/$(TOKEN).h --output=$(SRC_DIR)/$(PARSER).c $^ -v

$(BIN_DIR)/$(PARSER).o: $(SRC_DIR)/$(PARSER).c
	$(CC) $(CFLAGS) -c $^  -o $@

# codegen
$(BIN_DIR)/library.o: $(SRC_DIR)/library.c
	$(CC) $(CFLAGS) -c $< -o $@

# clean
clean:
	$(MAKE) clean-parser
	$(MAKE) clean-o
	$(MAKE) clean-exec
	$(MAKE) clean-test
	
clean-parser:
	rm include/token.h src/parser.c src/scanner.c src/parser.output 

clean-o:
	rm bin/*.o

clean-exec:
	rm $(BIN_DIR)/$(EXEC)

clean-test:
	rm -rf ./test/encode/good*.bminor.out
	rm -rf ./test/scanner/good*.bminor.out
	rm -rf ./test/parser/good*.bminor.out
	rm -rf ./test/printer/good*.bminor.a.out
	rm -rf ./test/printer/good*.bminor.b.out
	rm -rf ./test/printer/good*.bminor.out
	rm -rf ./test/resolve/good*.bminor.out
	rm -rf ./test/typecheck/good*.bminor.out
	rm -rf ./test/codegen/good*.bminor.out
	rm -rf ./test/codegen/good*.bminor.s
	rm -rf ./test/codegen/good*.bminor.exe

	rm -rf ./test/encode/bad*.bminor.out
	rm -rf ./test/scanner/bad*.bminor.out
	rm -rf ./test/parser/bad*.bminor.out
	rm -rf ./test/resolve/bad*.bminor.out
	rm -rf ./test/typecheck/bad*.bminor.out


# test
test:
	make
	./runtest.sh

test-encoder:
	make
	./bin/test_encoder_unit.sh

test-scanner:
	make
	./bin/test_scanner_unit.sh

test-parser:
	make
	./bin/test_parser_unit.sh

test-printer:
	make
	./bin/test_printer_unit.sh

test-resolver:
	make
	./bin/test_resolver_unit.sh

test-typechecker:
	make
	./bin/test_typechecker_unit.sh

.PHONY: test