
CFLAGS=-ggdb -O0  `pkg-config ExtremeCUnit --cflags` -fdevirtualize
LDLAGS=`pkg-config ExtremeCUnit --libs`

mock.o: mock.c

test: mock.o libunittest.so test.c
	$(CC) $(CFLAGS) -DUNIT_TEST -o $@ test.c mock.o libunittest.so -ldl
	
.PHONY: run
run: test
	export LD_LIBRARY_PATH=.;./test
