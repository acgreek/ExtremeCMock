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

#ifdef __cplusplus
#define MOCK_FUNC(srcFunc, dstFunc) mock_func(reinterpret_cast<void*>(srcFunc),reinterpret_cast<void*>(dstFunc))
#else 
#define MOCK_FUNC(srcFunc, dstFunc) mock_func((void*) srcFunc,(void*) dstFunc)
#endif
#define UNMOCK_FUNC(srcFunc, dstFunc) unmock_func((void*) srcFunc)
#define UNMOCK_ALL() unmock_all()

#endif 
