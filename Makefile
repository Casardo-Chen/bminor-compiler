CC = gcc
CFLAGS = -Wall -Wextra -std=c99
FUNC = encoder
MAIN = bminor
EXEC = bminor
TEST = runtest.sh

# Directories
TEST_DIR = test


$(EXEC): $(MAIN).o $(FUNC).o
	$(CC) $(CFLAGS) $(MAIN).o $(FUNC).o -o $(EXEC)
$(FUNC).o: $(FUNC).c $(FUNC).h
	$(CC) $(CFLAGS) -c $(FUNC).c -o $(FUNC).o
$(MAIN).o: $(MAIN).c $(FUNC).h
	$(CC) $(CFLAGS) -c $(MAIN).c -o $(MAIN).o
clean:
	rm $(EXEC)
	rm *.o
test:
	./runtest.sh

.PHONY: test