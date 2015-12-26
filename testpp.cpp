#include <stdlib.h>
#include <stdio.h>
#include "ExtremeCMock.h"
#include <ExtremeCUnit.h>
#define UNUSED __attribute__((unused))
int func2(UNUSED int arg1,UNUSED  int arg2) {
	return 20;
}
int  func1(UNUSED int arg1,UNUSED int arg2) {
	return 10;
}
int  func3(UNUSED int arg1,UNUSED int arg2) {
	return 30;
}


int  func_call3(UNUSED int arg1,UNUSED int arg2) {
	return 31 + func1(1,2);
}
int  func_call4(UNUSED int arg1,UNUSED int arg2) {
	return 32 + func_call3(1,2);
}
int  func_call5(UNUSED int arg1,UNUSED int arg2) {
	return 33 + func_call4(1,2);
}

TEST(mock_one_func_cpp) {
	Assert(func1(1,2) == 10);
	MOCK_FUNC(func1,func2);
	Assert(func1(1,2) == 20);
	UNMOCK_FUNC(func1);
	return 0;
}
TEST(mock_unmock_func_cpp) {
	Assert(func1(1,2) == 10);
	MOCK_FUNC(func1,func2);
	Assert(func1(1,2) == 20);
	UNMOCK_FUNC(func1);
	Assert(func1(1,2) == 10);
	return 0;
}
TEST(mock_remock_func_cpp) {
	Assert(func1(1,2) == 10);
	MOCK_FUNC(func1,func2);
	Assert(func1(1,2) == 20);
	MOCK_FUNC(func1,func3);
	Assert(func1(1,2) == 30);
	UNMOCK_FUNC(func1);
	Assert(func1(1,2) == 10);
	return 0;
}
TEST(mock_mock_call_from_func_cpp) {
	Assert(func1(1,2) == 10);
	Assert(func_call3(1,2) == 41);
	MOCK_FUNC(func1,func3);
	Assert(func1(1,2) == 30);
	Assert(func_call3(1,2) == 61);
	UNMOCK_FUNC(func1);
	Assert(func_call3(1,2) == 41);
	return 0;
}

TEST(mock_mock_call_that_is_mocked_cpp) {
	Assert(func_call5(1,2) == 106);
	MOCK_FUNC(func1,func3);
	Assert(func_call5(1,2) == 126);
	MOCK_FUNC(func_call4,func_call3);
	Assert(func_call5(1,2) == 94);
	MOCK_FUNC(func1,func2);
	Assert(func_call5(1,2) == 84);
	unmock_all();
	Assert(func_call5(1,2) == 106);
	return 0;
}

#include <unistd.h>

int my_read(UNUSED int fid,UNUSED  char * buffer,UNUSED int size) {
	return 90;
}

//extern int __strcmp_sse2(const char* a, const char* b) ;
//extern int __strcmp_sse42(const char* a, const char* b) ;
//extern int __strcmp_sse3(const char* a, const char* b) ;
TEST(mock_libc_cpp) {
	char buf[10];
	MOCK_FUNC(read,my_read);
	AssertEqInt(read(-1,buf, 9), 90);
	unmock_all();// make sure you do this, read maybe I should make the unit test frame work call this
	return 0;
}

class Foo_class {
	public:
	int method() {
		return 20;
	}
};

int method_bb(UNUSED Foo_class * objp) {
	return 30;
}

#ifndef __clang__
// clang++ does not allow member functions to be cast to nonmember functions
TEST(mock_class_member) {
	Foo_class a;
	AssertEqInt(a.method(), 20);
	MOCK_FUNC(&Foo_class::method,method_bb);
	AssertEqInt(a.method(), 30);
	unmock_all();// make sure you do this, read maybe I should make the unit test frame work call this
	return 0;
}
#endif

#ifdef __CYGWIN__
int main (int argc, char * argv[]){
	return windows_main(argc, argv);
}
#endif
