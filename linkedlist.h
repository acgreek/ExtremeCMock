#ifndef  LINKEDLIST_H
#define LINKEDLIST_H
#include <stddef.h>
#ifdef UNUSED 
#elif defined(__GNUC__) 
# define UNUSED __attribute__ ((unused))
#elif defined(__LCLINT__) 
# define UNUSED 
#else 
# define UNUSED 
#endif

typedef struct _ListNode_t {
	struct _ListNode_t *nextp;
	struct _ListNode_t *prevp;
} ListNode_t;

#define NODE_TO_ENTRY(STRUCTURE, ATTRIB,POINTER) \
	(STRUCTURE *)(((char *)POINTER)-offsetof(STRUCTURE,ATTRIB)) 
#define LINKEDLIST_FIRST(LISTP) LISTP.nextp
#define LINKEDLIST_LAST(LISTP) LISTP.prevp

#define LINKEDLIST_IS_EMPTY(LISTP) (LISTP.prevp == &LISTP) 

/**
 * initialize a linked list
 * you must call this before using a linked list
 *
 * @param listp  pointer to head of the Linked list 
 */
static inline void ListInitialize(ListNode_t * nodep) {
	nodep->nextp = nodep;
	nodep->prevp = nodep; 
}

/**
 * add a node to the end a linked list
 *
 * @assuption, linked list is initialized
 *
 * @param listp  pointer to head of the Linked list 
 * @param nodep  pointer to node to be removed
 */
static inline void ListAddEnd(ListNode_t * listp, ListNode_t *nodep) {
	nodep->nextp = listp;	
	nodep->prevp = listp->prevp;
	listp->prevp->nextp = nodep;
	listp->prevp = nodep;
}
static inline void ListAddEnd(ListNode_t * listp, ListNode_t *nodep) UNUSED;

/**
 * add a node to the end a linked list
 *
 * @assuption, linked list is initialized
 *
 * @param listp  pointer to head of the Linked list 
 * @param nodep  pointer to node to be removed
 */
static inline void ListAddBegin(ListNode_t * listp, ListNode_t *nodep) {
	nodep->nextp = listp->nextp;	
	nodep->prevp = listp;
	listp->nextp->prevp = nodep;
	listp->nextp = nodep;
}

static inline void ListAddBegin(ListNode_t * listp, ListNode_t *nodep) UNUSED;

#define ListInsertAfter ListAddBegin
#define ListInsertBefore ListAddEnd


/**
 * returns the number of elements in the lsit
 *
 * @param listp  pointer to head of the Linked list 
 *
 */
static inline int ListLength(ListNode_t * listp) {
	int count=0;
	ListNode_t *cur_nodep =listp->nextp;
	while (listp != cur_nodep) {
		cur_nodep= cur_nodep->nextp;
		count++;	
	}
	return count;
}

/**
 * Remove a node from the linked list
 *
 * @assuption, the node is in a linked list
 *
 * @param nodep  pointer to node to be removed
 */
static void ListRemove(ListNode_t * nodep)   {
	ListNode_t * nextp, *prevp;
	nextp = nodep->nextp;
	prevp = nodep->prevp;
	nextp->prevp = prevp;
	prevp->nextp = nextp;
}

static void ListRemove(ListNode_t * nodep) UNUSED;

/**
 * Apply function type 
 *
 * @param nodep  pointer to node
 * @param datap  pointer passed in as the second arg to function being applied
 */
typedef void ListApplyFunc_t(ListNode_t * nodep, void * datap);

/**
 * function use by ListFind, should return 1 on match and 0 on match
 *
 * @param nodep  pointer to node
 * @param datap  pointer passed in as the second arg to function being applied
 */
typedef int ListIsMatchFunc_t(ListNode_t * nodep, void * datap);

/**
 * applies the function provided by the func param to every node in the list
 *
 * @param listp  pointer to head of the Linked list 
 * @param func   function to apply to every node
 * @param datap  pointer passed in as the second arg to function being applied
 * 
 * @Note you should be able to safely remve the current node from with the func
 */
static void ListApplyAll(ListNode_t * listp, ListApplyFunc_t func, void *datap) {
	ListNode_t *cur_nodep =listp->nextp;
	while (listp != cur_nodep) {
		ListNode_t * nextp =cur_nodep->nextp ;
		func (cur_nodep, datap);
		cur_nodep= nextp;
	}

}
static void ListApplyAll(ListNode_t * listp, ListApplyFunc_t func, void *datap) UNUSED;

/**
 * applies the function provided by the func param to every node in the list
 *
 * @param listp  pointer to head of the Linked list 
 * @param func   function that will return 1 on match and 0 on not match 
 * @param datap  pointer passed in as the second arg to find function 
 * @return       pointer to first node that matches or NULL if non-match
 */
static ListNode_t * ListFind(ListNode_t * listp, ListIsMatchFunc_t func, void *datap) {
	ListNode_t *cur_nodep =listp->nextp;
	while (listp != cur_nodep) {
		ListNode_t * nextp =cur_nodep->nextp ;
		if (1 == func (cur_nodep, datap)) 
			return cur_nodep;
		cur_nodep= nextp;
	}
	return NULL;
}


/**
 * applies the function provided by the func param to every node in the list
 *
 * @param listp  pointer to head of the Linked list 
 * @param func   function to apply to every node
 * @param datap  pointer passed in as the second arg to function being applied
 *
 * @same a ListApplyAll 
 */
#define ListDestroyAll ListApplyAll

/**
 * should return 1 if ap > bp
 */
typedef int ListCompareFunc_t(ListNode_t * ap, ListNode_t *  dp, void * datap);

/**
 * sort a linked list
 */
static void ListSort(ListNode_t * listp, ListCompareFunc_t func, void *datap) {
	int length =ListLength (listp) +1;
	while (length) {
		ListNode_t *cur_nodep =listp->nextp;
		while (listp != cur_nodep && listp != cur_nodep->nextp) {
			ListNode_t * nextp =cur_nodep->nextp ;
			if (0 > func (cur_nodep, nextp,  datap)) {
				ListRemove(cur_nodep);			
				ListInsertAfter(nextp, cur_nodep);
			}
			else {
				cur_nodep=nextp;	
			}
		}
		length -=1;
	}
}	
static void ListSort(ListNode_t * listp, ListCompareFunc_t func, void *datap) UNUSED;

static void ListAddSorted(ListNode_t * listp,ListNode_t * nodep,  ListCompareFunc_t func, void *datap) {
	ListNode_t *cur_nodep =listp->nextp;
	while (listp != cur_nodep ) {
		if (0 < func (nodep, cur_nodep,  datap)) {
			ListInsertBefore(cur_nodep, nodep);
			return ;
		}
		cur_nodep =cur_nodep->nextp;
	}
	ListAddEnd(listp, nodep);
}
static void ListAddSorted(ListNode_t * listp,ListNode_t * nodep,  ListCompareFunc_t func, void *datap) UNUSED;


/**
 * returns the node in the list in which the compare function says is greater that or equal to
 */
static ListNode_t * ListFindLastGE(ListNode_t * listp,ListNode_t * nodep,  ListCompareFunc_t func, void *datap) {
	ListNode_t *cur_nodep =listp->nextp, *lastGE_nodep=NULL;
	while (listp != cur_nodep ) {
		if (0 > func (cur_nodep, nodep, datap)) {
			return lastGE_nodep;
		}
		lastGE_nodep = cur_nodep;
		cur_nodep =cur_nodep->nextp;
	}
	return NULL;
}
static ListNode_t * ListFindLastGE(ListNode_t * listp,ListNode_t * nodep,  ListCompareFunc_t func, void *datap) UNUSED;


#endif
