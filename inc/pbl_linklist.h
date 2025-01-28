/* define to prevent recursive inclusion -------------------------------------*/
#ifndef __PBL_LINKLIST_H
#define __PBL_LINKLIST_H

/* includes ------------------------------------------------------------------*/
#include "at32f425.h"


#ifdef __cplusplus
extern "C" {
#endif
/* define---------------------------------------------------------------------*/
typedef void pbl_ll_data_t;


typedef struct _pbl_ll_node_t {	
	pbl_ll_data_t *data;
	struct _pbl_ll_node_t *prev;
	struct _pbl_ll_node_t *next;		
} pbl_ll_node_t;

typedef struct _pbl_ll_t {
	uint32_t ll_len;	
	struct _pbl_ll_node_t *head;
	struct _pbl_ll_node_t *tail;
} pbl_ll_t;

/* exported functions ------------------------------------------------------- */
/**
  * @brief  链表初始化
  * @param  ll_p
  * @return head
  */
pbl_ll_t *pbl_ll_create(void);

/**
  * @brief  从头部插入一个节点
  * @param  ll_p：链表
  * @param  data：数据
  * @retval 新节点
  */
pbl_ll_node_t *pbl_ll_ins_head(pbl_ll_t *ll_p, pbl_ll_data_t *data);

/**
 * @brief Insert a new node in front of the n_act node
 * @param ll_p pointer to linked list
 * @param n_act pointer a node
 * @return pointer to the new node
 */
pbl_ll_node_t * pbl_ll_ins_prev(pbl_ll_t * ll_p, pbl_ll_node_t * n_act, pbl_ll_data_t *data);

/**
 * Add a new tail to a linked list
 * @param ll_p pointer to linked list
 * @return pointer to the new tail
 */
pbl_ll_node_t * pbl_ll_ins_tail(pbl_ll_t * ll_p, pbl_ll_data_t *data);

/**
 * Move a node before an other node in the same linked list
 * @param ll_p pointer to a linked list
 * @param n_act pointer to node to move
 * @param n_after pointer to a node which should be after `n_act`
 * @return none
 */
void pbl_ll_move_before(pbl_ll_t * ll_p, pbl_ll_node_t * n_act, pbl_ll_node_t * n_after);

/**
 * Remove the node 'node_p' from 'll_p' linked list.
 * It does not free the memory of node.
 * @param ll_p pointer to the linked list of 'node_p'
 * @param node_p pointer to node in 'll_p' linked list
 * @return none
 */
void pbl_ll_remove(pbl_ll_t * ll_p, pbl_ll_node_t *node_p);

/**
 * Remove and free all elements from a linked list. The list remain valid but become empty.
 * @param ll_p pointer to linked list
 * @return none
 */
void pbl_ll_clear(pbl_ll_t * ll_p);

 /**
 * Return with head node of the linked list
 * @param ll_p pointer to linked list
 * @return pointer to the head of 'll_p'
 */
pbl_ll_node_t *pbl_ll_get_head(const pbl_ll_t * ll_p);

/**
 * Return with tail node of the linked list
 * @param ll_p pointer to linked list
 * @return pointer to the tail of 'll_p'
 */
pbl_ll_node_t *pbl_ll_get_tail(const pbl_ll_t * ll_p);

/**
 * Return with the pointer of the next node after 'n_act'
 * @param ll_p pointer to linked list
 * @param n_act pointer a node
 * @return pointer to the next node
 */
pbl_ll_node_t * pbl_ll_get_next(const pbl_ll_node_t * n_act);

/**
 * Return with the pointer of the previous node before 'n_act'
 * @param ll_p pointer to linked list
 * @param n_act pointer a node
 * @return pointer to the previous node
 */
pbl_ll_node_t * pbl_ll_get_prev(const pbl_ll_node_t * n_act); 

/**
 * Return the length of the linked list.
 * @param ll_p pointer to linked list
 * @return length of the linked list
 */
uint32_t pbl_ll_get_len(const pbl_ll_t * ll_p);

/* extern variables-----------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif


