/**
	* �ļ���  staticLinkList.c
	* ����̬����
	* ��������������ӦΪ���󳤶�+2
	* ʱ�� 2022-8-4
**/


#include "staticLinkList.h"

/**
	*��̬������ʼ��
	*���� slink  ������С��Ϣ�ľ�̬����
				sll    ������̬����
				size   ������С
*/
uint8_t pbl_sll_init(pbl_sll_t *sll, pbl_sll_node_t *node_buff, uint16_t size) {	
	uint16_t i;
	
	sll->max_size = size;
	sll->node_buff = node_buff;
	for (i = 0; i  <  size-1; i++) {
		sll->node_buff[i].cur = i+1;
	}
	sll->node_buff[size-1].cur = 0;

	return 1;
}

/**
	*��̬�����ڴ����뺯��
*/

uint16_t pbl_sll_malloc(pbl_sll_t *sll) {
	uint16_t i = sll->node_buff[0].cur;
	
	if (sll->node_buff[0].cur) {
		sll->node_buff[0].cur = sll->node_buff[i].cur;
	}
	
	return i;
}

/**
	*��̬�����ڴ��ͷź���
*/

void pbl_sll_free(pbl_sll_t *sll, uint16_t pos) {
	sll->node_buff[pos].cur = sll->node_buff[0].cur;
	sll->node_buff[0].cur = pos;
}

/**
	*���ؾ�̬��������
*/
uint16_t pbl_sll_length(pbl_sll_t *sll) {
	uint16_t len = 0;
	uint16_t node_index = sll->node_buff[sll->max_size-1].cur;
	
	while (node_index) {
		node_index = sll->node_buff[node_index].cur;
		len++;
	}
	
	return len;
}

/**
	*��̬�������뺯��
*/
pbl_sll_node_t *pbl_sll_insert(pbl_sll_t *sll, uint16_t pos, pbl_sll_type data) {
	uint16_t new_node_index;
	uint16_t pre_node_index;
	uint16_t i;
	
	pre_node_index = sll->max_size - 1;
	if (pos < 1 || pos > pbl_sll_length(sll)+1)
		return 0;
	
	new_node_index = pbl_sll_malloc(sll);
	
	if (new_node_index) {
		sll->node_buff[new_node_index].data = data;
		for (i=1; i<=pos-1; i++) {
			pre_node_index = sll->node_buff[pre_node_index].cur;
		}
	
		sll->node_buff[new_node_index].cur = sll->node_buff[pre_node_index].cur;
		sll->node_buff[pre_node_index].cur = new_node_index;
		return &(sll->node_buff[new_node_index]);
	}

	return 0;
}

/**
	* ɾ����̬������i����Ա
*/
uint8_t pbl_sll_delete_pos(pbl_sll_t *sll, uint16_t pos) {
	uint16_t pre_node_index;
	uint16_t node_index;
	
	pre_node_index = sll->max_size - 1;
	if (pos < 1 || pos > pbl_sll_length(sll)+1)
		return 0;
	
	
	for (uint16_t i = 1; i < pos; i++) {
		pre_node_index = sll->node_buff[pre_node_index].cur;
	}	
	
	node_index = sll->node_buff[pre_node_index].cur;
	sll->node_buff[pre_node_index].cur = sll->node_buff[node_index].cur;
	pbl_sll_free(sll, node_index);

	return 1;
}

uint8_t pbl_sll_delete_node(pbl_sll_t *sll, pbl_sll_node_t *node) {
	uint16_t node_index;

	if (!node) {
		return 0;
	}

	node_index = sll->max_size - 1;
	while (sll->node_buff[node_index].cur != node->cur) {
		node_index = sll->node_buff[node_index].cur;
	}	

	pbl_sll_delete_pos(sll, node_index);
	return 1;
}

/**
	*��ȡ��̬������k����Ա����
*/

pbl_sll_type pbl_sll_get_data(pbl_sll_t *sll, uint16_t pos) {
	uint16_t node_index = sll->max_size - 1;

	for (uint16_t i = 0; i < pos; i++) {
		node_index = sll->node_buff[node_index].cur;
	}		

	return sll->node_buff[node_index].data;
}

pbl_sll_node_t *pbl_sll_get_head(pbl_sll_t *sll) {
	uint16_t head_index = sll->max_size - 1;

	return &(sll->node_buff[head_index]);
}

pbl_sll_node_t *pbl_sll_get_next(pbl_sll_t *slink, pbl_sll_node_t *node) {
	pbl_sll_node_t *next = &(slink->node_buff[node->cur]);

	return next;
}

uint16_t pbl_sll_get_head_index(pbl_sll_t *sll) {
	uint16_t node_index = sll->max_size - 1;

	return sll->node_buff[node_index].cur;
}

uint16_t pbl_sll_get_next_index(pbl_sll_t *slink, uint16_t pos) {
	return slink->node_buff[pos].cur;
}
