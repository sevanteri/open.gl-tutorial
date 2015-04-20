CC=gcc
CFLAGS= --std=c11 -Wall -DGLEW_STATIC -pedantic -pthread\
		-I../../fileWatcher/include/
LDLIBS= -lSDL2 -lGLEW -lGL -lm -lSOIL -lrt

OBJS = *.c ../../fileWatcher/watcher.o

all: 
	$(CC) $(CFLAGS) $(OBJS) $(LDLIBS) -o ogl
