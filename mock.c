#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
void func3();
int func2(int arg1, int arg2) {
	printf("start func2 %d %d\n",arg1,arg2);
	return 0;
}


void func1(int arg1,int arg2) {
	printf("start func1 %d %d\n",arg1, arg2);
	asm("pushq %rax\n\t"
	     "movq 0x0102030405060708, %rax\n\t"
	     "pushq %rax\n\t"
	     "retq\n\t");

	printf("end func1\n");
}
void mock(void * srcFunc, void * dstFunc) {
	char *p;
	long psize = sysconf(_SC_PAGESIZE);
	char jumpf[30] = "\x48\xb8XXXXXXXX\x50\xc3";
	char *addr = jumpf+2;
	(*(void**) (addr))=dstFunc; 
	for (p = (char *)srcFunc; (unsigned long)p % psize; --p)
		;
		
	if (-1 == mprotect(p,128, PROT_WRITE|PROT_READ|PROT_EXEC)){
		perror("could not set protection");
		exit(-1);
	}
	memcpy(srcFunc ,jumpf,5+sizeof(void*));

}
void func7() {
	printf("func7 s\n");
	func1(3,4 ); 
	printf("func7 e\n");
}
int main(void)
{
	int foo = 10, bar = 15;
	__asm__ __volatile__("addl  %%ebx,%%eax"
			:"=a"(foo)
			:"a"(foo), "b"(bar)
			);
	printf("foo+bar=%d\n", foo);
	mock(func1, func2);
	func7();
	printf("program end\n");

	return 0;
}
