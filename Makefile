CMP = gcc
FUNC = encoder
MAIN = bminor
EXEC = bminor

$(EXEC): $(MAIN).o $(FUNC).o
	$(CMP) $(MAIN).o $(FUNC).o -o $(EXEC)
$(FUNC).o: $(FUNC).c $(FUNC).h
	$(CMP) -c $(FUNC).c -o $(FUNC).o
$(MAIN).o: $(MAIN).c $(FUNC).h
	$(CMP) -c $(MAIN).c -o $(MAIN).o
clean:
	rm $(EXEC)
	rm *.o