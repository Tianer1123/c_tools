CC=gcc
INCL=-I./
CFLAGS=-Wall -c -g $(INCL)

TARGET=test

objects=main_test.o memory.o
source=main_test.c memory.c

all:
	$(CC) -c $(source) $(CFLAGS)
	$(CC) -o $(TARGET) $(objects)

clean:
	rm -f *.o a.out *~ *.a *.so $(TARGET)
