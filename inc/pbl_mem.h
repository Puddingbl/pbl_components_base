#ifndef __PBL_MEN_H__
#define __PBL_MEN_H__

#include "tlsf.h"
#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif

// If c99 or newer,  use the definition of uintptr_t directly from <stdint.h>
typedef uintptr_t pbl_uintptr_t;

/**
 * Heap information structure.
 */
typedef struct {
    uint32_t total_size; /**< Total heap size*/
    uint32_t free_cnt;
    uint32_t free_size; /**< Size of available memory*/
    uint32_t free_biggest_size;
    uint32_t used_cnt;
    uint32_t max_used; /**< Max size of Heap memory used*/
    uint8_t used_pct; /**< Percentage used*/
    uint8_t frag_pct; /**< Amount of fragmentation*/
} pbl_mem_monitor_t;


void pbl_mem_init(void);
void pbl_mem_deinit(void);
void *pbl_mem_alloc(size_t size);
void pbl_mem_free(void *data);
void *pbl_mem_realloc(void *data_p, size_t new_size);
void pbl_memset(void *dst, unsigned char v, size_t len);
void *pbl_memcpy(void *dst, const void *src, size_t len);
void pbl_mem_monitor(pbl_mem_monitor_t * mon_p);

#ifdef __cplusplus
}
#endif

#endif
