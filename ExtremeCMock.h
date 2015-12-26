#ifndef MOCK_H
#define MOCK_H

#ifdef __cplusplus
extern "C"  {
#endif
typedef void *(*mocked_func_cb)();

void mock_func(mocked_func_cb srcFunc, mocked_func_cb dstFunc);
void unmock_func(void * srcFunc());
void unmock_all();

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
#define MOCK_FUNC(srcFunc, dstFunc) mock_func(reinterpret_cast<mocked_func_cb>(srcFunc),reinterpret_cast<mocked_func_cb>(dstFunc))
#else
#define MOCK_FUNC(srcFunc, dstFunc) mock_func((mocked_func_cb)srcFunc,(mocked_func_cb)dstFunc)
#endif
#define UNMOCK_FUNC(srcFunc) unmock_func((mocked_func_cb) srcFunc)
#define UNMOCK_ALL() unmock_all()

#endif
