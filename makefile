CXXFLAGS=-ggdb -Iinclude -D_FILE_OFFSET_BITS=64 -fmax-errors=16 -pedantic -Wall -Werror

test: test.o src/fuse++.o
	g++ -ggdb $^ -o $@ -lfuse

test.o src/fuse++.o: include/*

clean:
	-rm *.o test
