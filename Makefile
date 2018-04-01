TARGET = level1 level2
LIBS = -lm -lcurses
CC = gcc
CFLAGS = -g -Wall

.PHONY: default
.PRECIOUS: $(TARGET) $(OBJECTS)
default: $(TARGET)

MAINS=level1.o level2.o
OBJECTS = $(filter-out $(MAINS), $(patsubst %.c, %.o, $(wildcard *.c)))
HEADERS = $(wildcard *.h)

level1: level1.o $(OBJECTS)
	$(CC) $(OBJECTS) level1.o -Wall $(LIBS) -o level1

level2: level2.o $(OBJECTS)
	$(CC) $(OBJECTS) level2.o -Wall $(LIBS) -o level2

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	-rm -f *.o
	-rm -f $(TARGET)
