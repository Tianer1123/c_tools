CC=gcc
INCL=-I./
CFLAGS=-c -g $(INCL)

TARGET=acsm2

objects=test.o acsmx2.o
source=test.c acsmx2.c

all:
	$(CC) -c $(source) $(CFLAGS)
	$(CC) -o $(TARGET) $(objects)

clean:
	rm -f *.o acsm *~ *.a *.so $(TARGET)
