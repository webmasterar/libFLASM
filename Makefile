CC = g++
RM = rm -f
RMDIR = rmdir

default: main

main: libflasm.o
	$(CC) -shared -o libflasm.so libflasm.o

libflasm.o:
	$(CC) -I . -std=c++11 -g -D_USE_64 -msse4.2 -O3 -fomit-frame-pointer -funroll-loops -DNDEBUG -c -fpic libflasm.cpp -o libflasm.o

clean:
	$(RM) libflasm.so libflasm.o

clean-all:
	$(RM) libflasm.so libflasm.o
	$(RM) -r seqan/*
	$(RMDIR) seqan
