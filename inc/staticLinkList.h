/* define to prevent recursive inclusion -------------------------------------*/
#ifndef __STATICLINKLIST_H
#define __STATICLINKLIST_H

#ifdef __cplusplus
extern "C" {
#endif

/* includes ------------------------------------------------------------------*/
// #include <stdio.h>
#include "at32f425.h"
/* define---------------------------------------------------------------------*/
typedef     void*     pbl_sll_type; 

typedef struct {
	pbl_sll_type 	data;
	uint16_t 		cur;
} pbl_sll_node_t;

typedef struct {
	pbl_sll_node_t	  *node_buff;
	uint16_t  max_size;
} pbl_sll_t;

/* extern variables-----------------------------------------------------------*/

/* exported functions ------------------------------------------------------- */
uint8_t pbl_sll_init(pbl_sll_t *sll, pbl_sll_node_t *node_buff, uint16_t size);
uint16_t pbl_sll_malloc(pbl_sll_t *sll);
void pbl_sll_free(pbl_sll_t *sll, uint16_t pos);
uint16_t pbl_sll_length(pbl_sll_t *sll);
pbl_sll_node_t *pbl_sll_insert(pbl_sll_t *sll, uint16_t pos, pbl_sll_type data);
uint8_t pbl_sll_delete_pos(pbl_sll_t *sll, uint16_t pos);
uint8_t pbl_sll_delete_node(pbl_sll_t *sll, pbl_sll_node_t *node); 
pbl_sll_type pbl_sll_get_data(pbl_sll_t *sll, uint16_t pos);
pbl_sll_node_t *pbl_sll_get_head(pbl_sll_t *sll);
pbl_sll_node_t *pbl_sll_get_next(pbl_sll_t *sll, pbl_sll_node_t *node);
uint16_t pbl_sll_get_head_index(pbl_sll_t *sll);
uint16_t pbl_sll_get_next_index(pbl_sll_t *sll, uint16_t pos);


#ifdef __cplusplus
}
#endif

#endif
