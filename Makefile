CC=gcc
CFLAGS= --std=c11 -Wall -DGLEW_STATIC -pedantic
LDLIBS= -lSDL2 -lGLEW -lGL -lm -lSOIL

OBJS = *.c

all: 
	$(CC) $(CFLAGS) $(OBJS) $(LDLIBS) -o eka
