
CFLAGS=-ggdb -O0  -ldl -fPIC -pie -rdynamic -fdevirtualize
LDLAGS=-ldl

mock.o: mock.c

test: mock.o libunittest.so test.c unittest.h
	$(CC) $(CFLAGS) -DUNIT_TEST -o $@ test.c mock.o libunittest.so -ldl
	
.PHONY: run
run: test
	export LD_LIBRARY_PATH=.;./test
