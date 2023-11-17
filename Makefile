# Author: Meng Chen
# Date: 10/10/2023

###############
# c compilers #
###############
CC = gcc
CFLAGS = -Wall 

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

###############
# Directories #
###############
TEST_DIR = test


###############
# 	  make    #
###############
$(EXEC): $(MAIN).o $(HELPER).o $(ENCODER).o $(SCANNER).o $(PARSER).o $(EXPR).o $(STMT).o $(PARAM).o $(DECL).o $(TYPE).o $(SYM).o $(SCOPE).o $(HT).o
	$(CC) $(CFLAGS) -o $@ $^ 

# encoder
$(ENCODER).o: $(ENCODER).c $(ENCODER).h
	$(CC) $(CFLAGS) -c $<  -o $@

# scanner
$(SCANNER).c: $(SCANNER).l
	flex -o $@ $^

$(SCANNER).o: $(SCANNER).c $(TOKEN).h
	$(CC) -c $<  -o $@

# parser
$(PARSER).c $(TOKEN).h: $(PARSER).y
	bison --defines=$(TOKEN).h --output=$(PARSER).c $^ -v

$(PARSER).o: $(PARSER).c
	$(CC) $(CFLAGS) -c $^  -o $@

$(HELPER).o: $(HELPER).c $(HELPER).h
	$(CC) $(CFLAGS) -c $(HELPER).c -o $(HELPER).o
	
$(MAIN).o: $(MAIN).c $(TOKEN).h
	$(CC) $(CFLAGS) -c $<  -o $@

# printer
$(EXPR).o: $(EXPR).c $(EXPR).h 
	$(CC) $(CFLAGS) -c $<  -o $@

$(STMT).o: $(STMT).c $(STMT).h 
	$(CC) $(CFLAGS) -c $<  -o $@

$(PARAM).o: $(PARAM).c $(PARAM).h 
	$(CC) $(CFLAGS) -c $<  -o $@

$(DECL).o: $(DECL).c $(DECL).h 
	$(CC) $(CFLAGS) -c $<  -o $@

$(TYPE).o: $(TYPE).c $(TYPE).h 
	$(CC) $(CFLAGS) -c $<  -o $@

# resolve
$(SYM).o: $(SYM).c $(SYM).h
	$(CC) $(CFLAGS) -c $<  -o $@

$(SCOPE).o: $(SCOPE).c $(SCOPE).h
	$(CC) $(CFLAGS) -c $<  -o $@

$(HT).o: $(HT).c $(HT).h
	$(CC) $(CFLAGS) -c $<  -o $@

# clean
clean-parser:
	rm token.h parser.c scanner.c

clean-o:
	rm *.o

clean-exec:
	rm $(EXEC)

clean-test:
	rm -rf ./test/encode/good*.bminor.out
	rm -rf ./test/scanner/good*.bminor.out
	rm -rf ./test/parser/good*.bminor.out
	rm -rf ./test/printer/good*.bminor.a.out
	rm -rf ./test/printer/good*.bminor.b.out
	rm -rf ./test/printer/good*.bminor.out
	rm -rf ./test/resolve/good*.bminor.out
	rm -rf ./test/typecheck/good*.bminor.out

	rm -rf ./test/encode/bad*.bminor.out
	rm -rf ./test/scanner/bad*.bminor.out
	rm -rf ./test/parser/bad*.bminor.out
	rm -rf ./test/resolve/bad*.bminor.out
	rm -rf ./test/typecheck/bad*.bminor.out
	

clean:
	rm token.h 
	rm parser.c 
	rm scanner.c 
	rm parser.output 
	rm *.o
	rm $(EXEC)

# test
test:
	make
	./runtest.sh

.PHONY: test