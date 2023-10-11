# Author: Meng Chen
# Date: 10/10/2023

###############
# c compilers #
###############
CC = gcc
CFLAGS = -Wall -Wextra -std=c99

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


###############
# Directories #
###############
TEST_DIR = test


###############
# 	  make    #
###############
$(EXEC): $(MAIN).o $(HELPER).o $(ENCODER).o $(SCANNER).o $(PARSER).o
	$(CC) $(CFLAGS) -o $@ $^ 

# encoder
$(ENCODER).o: $(ENCODER).c $(ENCODER).h
	$(CC) $(CFLAGS) -c $<  -o $@

# scanner
$(SCANNER).c: $(SCANNER).flex
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
	rm -rf ./test/encode/bad*.bminor.out
	rm -rf ./test/scanner/bad*.bminor.out
	rm -rf ./test/parser/bad*.bminor.out

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