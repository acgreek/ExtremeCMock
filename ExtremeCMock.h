#ifndef MOCK_H
#define MOCK_H

void mock_func(void * srcFunc, void * dstFunc);
void unmock_func(void * srcFunc);
void unmock_all();

#endif 
