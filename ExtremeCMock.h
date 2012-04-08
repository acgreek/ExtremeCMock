#ifndef MOCK_H
#define MOCK_H

#ifdef __cplusplus
extern "C"  {
#endif
void mock_func(void * srcFunc, void * dstFunc);
void unmock_func(void * srcFunc);
void unmock_all();
#ifdef __cplusplus
}
#endif

#define MOCK_FUNC(srcFunc, dstFunc) mock_func((void*) srcFunc,(void*) dstFunc)
#define UNMOCK_FUNC(srcFunc, dstFunc) unmock_func((void*) srcFunc)
#define UNMOCK_ALL() unmock_all()

#endif 
