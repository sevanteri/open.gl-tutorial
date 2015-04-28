CC=gcc
CFLAGS= --std=c11 -Wall -DGLEW_STATIC -pedantic -O3
LDLIBS= -lSDL2 -lGLEW -lGL -lm -lSOIL -lrt -lgraphene-1.0

OBJS = *.c

all: 
	$(CC) $(CFLAGS) $(OBJS) $(LDLIBS) -o ogl
