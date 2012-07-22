#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include "linkedlist.h"

static int lg_initialized=0;
static ListNode_t lg_mocked_functions_list;

static void initialize_mock_api() {
	ListInitialize(&lg_mocked_functions_list);
	lg_initialized=1;
}

#define STUB_SIZE 5+sizeof(void*)

typedef struct _mocked_function_t  {
	ListNode_t node;
	void * addr;
	void * new_addr;
	char backup_function_data[STUB_SIZE];
} mocked_function_t;

static int isMatchMockedFunction (ListNode_t* n, void * addr) {
	mocked_function_t *cur_a = NODE_TO_ENTRY(mocked_function_t,node, n);
	if (addr == cur_a->addr)
		return 1;
	return 0;
}

static mocked_function_t *
findMockedFunction(void * addr) {
	ListNode_t * foundp;
	if (0 ==lg_initialized) {
		initialize_mock_api();
	}
	foundp =ListFind(&lg_mocked_functions_list,isMatchMockedFunction ,addr);
	if (NULL == foundp)
		return NULL;
	return NODE_TO_ENTRY(mocked_function_t,node, foundp);
}

static void unprotect_address(mocked_function_t * functionp) {
	char *p;
	long psize = sysconf(_SC_PAGESIZE);
	for (p = (char *)functionp->addr; (unsigned long)p % psize; --p)
		;
	if (-1 == mprotect(p,128, PROT_WRITE|PROT_READ|PROT_EXEC)){
		perror("could not set protection");
		exit(-1);
	}
	memcpy(functionp->backup_function_data,functionp->addr,STUB_SIZE);
}

static mocked_function_t * new_mocked_function(void * addr) {
	mocked_function_t * mfp;
	mfp = malloc (sizeof(mocked_function_t));
	mfp->addr = addr;
	mfp->new_addr = NULL;
	ListAddBegin(&lg_mocked_functions_list, &mfp->node);
	unprotect_address(mfp);
	return mfp;
}

static void hi_jack_function(mocked_function_t *functionp,void * dstFunc) {
#ifdef __amd64__
	char jumpf[30] = "\x48\xb8XXXXXXXX\x50\xc3";
	char *addr = jumpf+2;
	(*(void**) (addr))=dstFunc;
	memcpy(functionp->addr,jumpf,STUB_SIZE);
#elif __i386__
#define HIJACK_ADDR(x) *(void**)((x)+1)
	static const char hijack_stub[] = {
	0x68, 0x00, 0x00, 0x00, 0x00, /* push addr */
	0xc3                          /* ret */
	};
	memcpy(functionp->addr, hijack_stub, sizeof(hijack_stub));
	HIJACK_ADDR(functionp->addr) = dstFunc;

#else
	abort();
#endif
}
static void unhi_jack_function(mocked_function_t *functionp) {
	memcpy(functionp->addr,functionp->backup_function_data,STUB_SIZE);
	ListRemove ( &functionp->node);
	free(functionp);

}
/**
 * hi-jacks a function
 * @param address to hi-jack
 * @param address to hi-jack with
 */
void mock_func(void * srcFunc, void * dstFunc) {
	mocked_function_t *functionp;

	functionp = findMockedFunction(srcFunc);
	if (NULL == functionp)
		functionp = new_mocked_function(srcFunc);

	if (dstFunc == functionp->new_addr)
		return ; // already hi_jacked to this address

	hi_jack_function(functionp, dstFunc);
}

/**
 * unhi-jacks a function
 * @param address to un-hi-jack
 */
void unmock_func(void * srcFunc) {
	mocked_function_t *functionp;
	functionp = findMockedFunction(srcFunc);
	if (NULL == functionp)
		return;
	unhi_jack_function(functionp);
}

static void unhi_jack_list_node(ListNode_t *foundp, UNUSED void * foo) {
	unhi_jack_function( NODE_TO_ENTRY(mocked_function_t,node, foundp));
}
/**
 * unhi-jacks all hi-jacked  functions
 */
void unmock_all() {
	ListApplyAll(&lg_mocked_functions_list,unhi_jack_list_node, NULL);
}
