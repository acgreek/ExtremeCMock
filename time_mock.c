#include <stdio.h>
#include <time.h>
#include "ExtremeCMock.h"

long time_mock() {
	static int now = 10;
	return now++;
}

int main(void) {
	printf("time is now %lu\n",(unsigned long ) time(NULL));
	MOCK_FUNC( time,time_mock);
	printf("time is now %lu\n", (unsigned long )time(NULL));
	printf("time is now %lu\n",(unsigned long ) time(NULL));
	unmock_all();
	printf("time is now %lu\n",(unsigned long ) time(NULL));
	return 0;
}
