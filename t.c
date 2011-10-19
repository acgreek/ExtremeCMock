#include <sys/types.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

void
foo(void) {
	puts("hello");
}

int
main(void) {
	char    *p;
	long    psize = sysconf(_SC_PAGESIZE);

	if (mprotect(foo, 128, PROT_READ|PROT_WRITE|PROT_EXEC) == -1) {
		perror("mprotect");
	}
	for (p = (char *)foo; (unsigned long)p % psize; --p)
		;
	if (mprotect(p, 128, PROT_READ|PROT_WRITE|PROT_EXEC) == -1) {
		perror("mprotect");
	} else {
		puts("Alright, that worked");
		strcpy(p, "hello");
	}
	return 0;
}
