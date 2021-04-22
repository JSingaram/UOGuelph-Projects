CC= gcc
CFLAGS= -c -Wall -ansi
OBJ= fleschIndex.o

fleschIndex: $(OBJ)
	$(CC) $(OBJ) -o fleschIndex

fleschIndex.o: fleschIndex.c
	$(CC) $(CFLAGS) fleschIndex.c

clean:
	rm fleschIndex fleschIndex.o