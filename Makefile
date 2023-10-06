# c compilers
CC = gcc
CFLAGS = -Wall -Wextra -std=c99

# files
ENCODER = encoder
SCANNER = scanner
MAIN = main
HELPER = bminor_helper
EXEC = bminor
TEST = runtest.sh

# Directories
TEST_DIR = test


$(EXEC): $(MAIN).o $(ENCODER).o $(SCANNER).o $(HELPER).o
	$(CC) $(CFLAGS) $(MAIN).o $(ENCODER).o $(SCANNER).o $(HELPER).o -o $(EXEC) 
$(ENCODER).o: $(ENCODER).c $(ENCODER).h
	$(CC) $(CFLAGS) -c $(ENCODER).c -o $(ENCODER).o

bminor_helper.o: $(HELPER).c $(HELPER).h
	$(CC) $(CFLAGS) -c $(HELPER).c -o $(HELPER).o

$(SCANNER).c: $(SCANNER).flex
	flex -o $(SCANNER).c $(SCANNER).flex

$(SCANNER).o: $(SCANNER).c
	$(CC) $(CFLAGS) -o $(SCANNER).o -c $(SCANNER).c
	
$(MAIN).o: $(MAIN).c
	$(CC) $(CFLAGS) -c $(MAIN).c  -o $(MAIN).o 

clean:
	rm $(EXEC)
	rm *.o
test:
	./runtest.sh

.PHONY: test