CC=gcc
CFLAGS= --std=c11 -Wall -DGLEW_STATIC -pedantic -O3 -pthread
LDLIBS= -lSDL2 -lGLEW -lGL -lm -lSOIL -lrt

OBJS = *.c

all: 
	$(CC) $(CFLAGS) $(OBJS) $(LDLIBS) -o ogl
