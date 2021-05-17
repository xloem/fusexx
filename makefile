CXXFLAGS=-ggdb -Iinclude -D_FILE_OFFSET_BITS=64 -fmax-errors=16 -pedantic -Wall -Werror $$(pkg-config --cflags fuse3 --silence-errors || pkg-config --cflags fuse)
LDFLAGS=$$(pkg-config --ldflags fuse3 --silence-errors || pkg-config --ldflags fuse)

test: test.o src/fuse++.o
	g++ -ggdb $^ -o $@ $(LDFLAGS)

test.o src/fuse++.o: include/*

clean:
	-rm *.o test
