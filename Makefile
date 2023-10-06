CC = gcc
CFLAGS = -Wall -Wextra -std=c99
FUNC = encoder
MAIN = main
EXEC = bminor
TEST = runtest.sh

# Directories
TEST_DIR = test


$(EXEC): $(MAIN).o $(FUNC).o scanner.o scanner_helper.o
	$(CC) $(CFLAGS) $(MAIN).o $(FUNC).o scanner.o scanner_helper.o -o $(EXEC) 
$(FUNC).o: $(FUNC).c $(FUNC).h
	$(CC) $(CFLAGS) -c $(FUNC).c -o $(FUNC).o

scanner_helper.o: scanner_helper.c scanner_helper.h
	$(CC) $(CFLAGS) -c scanner_helper.c -o scanner_helper.o

scanner.c: scanner.flex
	flex -o scanner.c scanner.flex

scanner.o: scanner.c
	$(CC) -o scanner.o -c scanner.c
	
$(MAIN).o: $(MAIN).c
	$(CC) -c $(MAIN).c  -o $(MAIN).o 
clean:
	rm $(EXEC)
	rm *.o
test:
	./runtest.sh

.PHONY: test