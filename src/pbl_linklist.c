/**
 * 文件名：pbl_linklist.c
 * 时间：20250125
 * 作者：pbl
**/
#include "pbl_linklist.h"
#include "pbl_mem.h"



#define LL_NODE_MEM_ALLOC pbl_mem_alloc(sizeof(pbl_ll_node_t *))

/**
  * @brief  链表初始化
  * @param  ll_p
  * @return head
  */
pbl_ll_t *pbl_ll_create(void) {	
	pbl_ll_t *ll_p = pbl_mem_alloc(sizeof(pbl_ll_t *));
	ll_p->head = NULL;
	ll_p->tail = NULL;	
	ll_p->ll_len = 0;
	
	return ll_p;
}

/**
  * @brief  从头部插入一个节点
  * @param  ll_p：链表
  * @param  data：数据
  * @retval 新节点
  */
pbl_ll_node_t *pbl_ll_ins_head(pbl_ll_t *ll_p, pbl_ll_data_t *data) {	
    pbl_ll_node_t * n_new;
	pbl_ll_node_t *n_prev = NULL;
	
    n_new = LL_NODE_MEM_ALLOC;	
	if (n_new == NULL) {
		return NULL;
	}
	ll_p->ll_len++;
	
	n_new->prev = NULL;
	n_new->next = ll_p->head;
	n_prev = ll_p->head;
	n_prev->prev = n_new;	
	ll_p->head = n_new;
	
	if(ll_p->tail == NULL) { /*If there is no tail (1. node) set the tail too*/
		ll_p->tail = n_new;
	}
	
	n_new->data = data;
	
	return n_new;
}

/**
 * @brief Insert a new node in front of the n_act node
 * @param ll_p pointer to linked list
 * @param n_act pointer a node
 * @return pointer to the new node
 */
pbl_ll_node_t * pbl_ll_ins_prev(pbl_ll_t * ll_p, pbl_ll_node_t * n_act, pbl_ll_data_t *data) {
    pbl_ll_node_t * n_new;

    if (NULL == ll_p || NULL == n_act) return NULL;	
	
	if (pbl_ll_get_head(ll_p) == n_act) {
		n_new = pbl_ll_ins_head(ll_p, data);
		if (n_new == NULL) {
			return NULL;
		} 
	} else {
		n_new = LL_NODE_MEM_ALLOC;
		if (n_new == NULL) {
			return NULL;
		} 
		ll_p->ll_len++;
		
		pbl_ll_node_t *n_prev = pbl_ll_get_prev(n_act);
		n_prev->next = n_new;
		n_new->prev = n_prev;
		n_act->prev = n_new;
		n_new->next = n_act;	
		
		n_new->data = data;	
	}
	
    return n_new;
}

/**
 * Add a new tail to a linked list
 * @param ll_p pointer to linked list
 * @return pointer to the new tail
 */
pbl_ll_node_t * pbl_ll_ins_tail(pbl_ll_t * ll_p, pbl_ll_data_t *data) {
    pbl_ll_node_t * n_new;
	pbl_ll_node_t *n_next = NULL;
	
    n_new = LL_NODE_MEM_ALLOC;
	if (n_new == NULL) {
		return NULL;
	}
	ll_p->ll_len++;
	
	n_new->prev = ll_p->tail;
	n_new->next = NULL;
	n_next = ll_p->tail;
	n_next->next = n_new;
	ll_p->tail = n_new;
	
	if(ll_p->head == NULL) { /*If there is no head (1. node) set the head too*/
		ll_p->head = n_new;
	}
	n_new->data = data;

    return n_new;
}

/**
 * Move a node before an other node in the same linked list
 * @param ll_p pointer to a linked list
 * @param n_act pointer to node to move
 * @param n_after pointer to a node which should be after `n_act`
 * @return none
 */
void pbl_ll_move_before(pbl_ll_t * ll_p, pbl_ll_node_t * n_act, pbl_ll_node_t * n_after) {
	/*Can't move before itself*/
    if (n_act == n_after) {
		return; 
	} 

    pbl_ll_node_t * n_before;
    if (n_after != NULL) {
        n_before = pbl_ll_get_prev(n_after);
	} else {
        n_before = pbl_ll_get_tail(ll_p); /*if `n_after` is NULL `n_act` should be the new tail*/
	}

	/*Already before `n_after`*/
    if (n_act == n_before) { 
		return;
	} 

    /*It's much easier to remove from the list and add again*/
    pbl_ll_remove(ll_p, n_act);
	ll_p->ll_len++;
	
    /*Add again by setting the prev. and next nodes*/
	n_before->next = n_act;
	n_act->prev = n_before;
	n_after->prev = n_act;
	n_act->next = n_after;
	
    /*If `n_act` was moved before NULL then it become the new tail*/
    if (n_after == NULL) ll_p->tail = n_act;

    /*If `n_act` was moved before `NULL` then it's the new head*/
    if (n_before == NULL) ll_p->head = n_act;
}

/**
 * Remove the node 'node_p' from 'll_p' linked list.
 * It does not free the memory of node.
 * @param ll_p pointer to the linked list of 'node_p'
 * @param node_p pointer to node in 'll_p' linked list
 * @return none
 */
void pbl_ll_remove(pbl_ll_t * ll_p, pbl_ll_node_t *node_p) {
    if (ll_p == NULL) {
		return;
	} 

    if (pbl_ll_get_head(ll_p) == node_p) {
        /*The new head will be the node after 'n_act'*/
        ll_p->head = pbl_ll_get_next(node_p);
        if (ll_p->head == NULL) {
            ll_p->tail = NULL;
        } else {
			ll_p->head = node_p->next;
        }
    } else if (pbl_ll_get_tail(ll_p) == node_p) {
        /*The new tail will be the node before 'n_act'*/
        ll_p->tail = pbl_ll_get_prev(node_p);
        if (ll_p->tail == NULL) {
            ll_p->head = NULL;
        } else {
            ll_p->tail = node_p->prev;
        }
    } else {
        pbl_ll_node_t * n_prev = pbl_ll_get_prev(node_p);
        pbl_ll_node_t * n_next = pbl_ll_get_next(node_p);

		n_prev->next = node_p->next;
		n_next->prev = node_p->prev;
    }
	ll_p->ll_len--;
}

/**
 * Remove and free all elements from a linked list. The list remain valid but become empty.
 * @param ll_p pointer to linked list
 * @return none
 */
void pbl_ll_clear(pbl_ll_t * ll_p) {
    pbl_ll_node_t * node;
    pbl_ll_node_t * n_next;

    node = pbl_ll_get_head(ll_p);
    n_next = NULL;

    while (node != NULL) { 
        n_next = pbl_ll_get_next(node);

        pbl_ll_remove(ll_p, node);
        pbl_mem_free(node);

        node = n_next;
    }
}

/**
 * Return with head node of the linked list
 * @param ll_p pointer to linked list
 * @return pointer to the head of 'll_p'
 */
pbl_ll_node_t *pbl_ll_get_head(const pbl_ll_t * ll_p) {
    if(ll_p == NULL) return NULL;
    return ll_p->head;
}

/**
 * Return with tail node of the linked list
 * @param ll_p pointer to linked list
 * @return pointer to the tail of 'll_p'
 */
pbl_ll_node_t *pbl_ll_get_tail(const pbl_ll_t * ll_p) {
    if(ll_p == NULL) return NULL;
    return ll_p->tail;
}

/**
 * Return with the pointer of the next node after 'n_act'
 * @param ll_p pointer to linked list
 * @param n_act pointer a node
 * @return pointer to the next node
 */
pbl_ll_node_t * pbl_ll_get_next(const pbl_ll_node_t * n_act) {
    return n_act->next;
}

/**
 * Return with the pointer of the previous node before 'n_act'
 * @param ll_p pointer to linked list
 * @param n_act pointer a node
 * @return pointer to the previous node
 */
pbl_ll_node_t * pbl_ll_get_prev(const pbl_ll_node_t * n_act) {
    return n_act->prev;
}

/**
 * Return the length of the linked list.
 * @param ll_p pointer to linked list
 * @return length of the linked list
 */
uint32_t pbl_ll_get_len(const pbl_ll_t * ll_p) {
    return ll_p->ll_len;
}
