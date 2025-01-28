#include "pbl_mem.h"
#include "pbl_tlsf_conf.h"
#include "tlsf.h"
//
//#include "public.h"

#ifdef  PBL_ARCH_64
#define MEM_UNIT         uint64_t
#define ALIGN_MASK       0x7
#else
#define MEM_UNIT         uint32_t
#define ALIGN_MASK       0x3
#endif

#define PBL_MAX(a, b) ((a) > (b) ? (a) : (b))

#define ZERO_MEM_SENTINEL  (unsigned int*)0xa1b2c3d4


#define COPY32 *d32 = *s32; d32++; s32++;
#define COPY8 *d8 = *s8; d8++; s8++;
#define SET32(x) *d32 = x; d32++;
#define SET8(x) *d8 = x; d8++;
#define REPEAT8(expr) expr expr expr expr expr expr expr expr

#if PBL_MEM_MONITOR
static void pbl_mem_walker(void * ptr, size_t size, int used, void * user);
#endif


/**********************
 *  STATIC VARIABLES
 **********************/
static tlsf_t tlsf;
static  MEM_UNIT work_mem_int[PBL_MEM_SIZE / sizeof(MEM_UNIT)];

#if PBL_MEM_MONITOR	
static uint32_t cur_used;
static uint32_t max_used;
#endif
/**
 * Initialize the dyn_mem module (work memory and other variables)
 */
void pbl_mem_init(void) {	
	tlsf = tlsf_create_with_pool((void *)work_mem_int, PBL_MEM_SIZE);
}

/**
 * Clean up the memory buffer which frees all the allocated memories.
 * @note It work only if `LV_MEM_CUSTOM == 0`
 */
void pbl_mem_deinit(void) {

	tlsf_destroy(tlsf);
//	mem_init();
}

/**
 * Allocate a memory dynamically
 * @param size size of the memory to allocate in bytes
 * @return pointer to the allocated memory
 */
void *pbl_mem_alloc(size_t size) {
	if (size == 0) {
		return ZERO_MEM_SENTINEL;
	}   
    
	void *alloc = tlsf_malloc(tlsf, size);

	if (alloc == NULL) {
        PBL_MEM_LOG_E("alloc == NULL");
	}

#if PBL_MEM_MONITOR
    pbl_mem_monitor_t mon;
    pbl_mem_monitor(&mon);
    PBL_MEM_LOG_D("used: %6d (%3d %%), frag: %3d %%, biggest free: %6d",
                (int)(mon.total_size - mon.free_size), mon.used_pct, mon.frag_pct,
                (int)mon.free_biggest_size);    
#endif    
    if(alloc) {
#if PBL_MEM_MONITOR
        cur_used += size;
        max_used = PBL_MAX(cur_used, max_used);
		PBL_MEM_LOG_D("allocated at %p", alloc);
#endif      
    }
    
	return alloc;
}

/**
 * Free an allocated data
 * @param data pointer to an allocated memory
 */
void pbl_mem_free(void *data) {
	if (data == ZERO_MEM_SENTINEL)
		return;
	if (data == NULL)
		return;

	size_t size = tlsf_free(tlsf, data);
#if PBL_MEM_MONITOR	
    if(cur_used > size) cur_used -= size;
    else cur_used = 0;    
#endif  	
}

/**
 * Reallocate a memory with a new size. The old content will be kept.
 * @param data pointer to an allocated memory.
 * Its content will be copied to the new memory block and freed
 * @param new_size the desired new size in byte
 * @return pointer to the new memory
 */
void *pbl_mem_realloc(void *data_p, size_t new_size) {
	if (new_size == 0) {       
		pbl_mem_free(data_p);
		return ZERO_MEM_SENTINEL;
	}

	if (data_p == ZERO_MEM_SENTINEL)
		return pbl_mem_alloc(new_size);


	void *new_p = tlsf_realloc(tlsf, data_p, new_size);

	if (new_p == NULL) {
        PBL_MEM_LOG_E("alloc == NULL");
		return NULL;
	}

	return new_p;
}

#if PBL_MEM_MONITOR
/**
 * Give information about the work memory of dynamic allocation
 * @param mon_p pointer to a lv_mem_monitor_t variable,
 *              the result of the analysis will be stored here
 */
void pbl_mem_monitor(pbl_mem_monitor_t * mon_p)
{
    /*Init the data*/
    pbl_memset(mon_p, 0, sizeof(pbl_mem_monitor_t));
//    DEBUG_LOG("begin");

    tlsf_walk_pool(tlsf_get_pool(tlsf), pbl_mem_walker, mon_p);

    mon_p->total_size = PBL_MEM_SIZE;
    mon_p->used_pct = 100 - (100U * mon_p->free_size) / mon_p->total_size;
    if(mon_p->free_size > 0) {
        mon_p->frag_pct = mon_p->free_biggest_size * 100U / mon_p->free_size;
        mon_p->frag_pct = 100 - mon_p->frag_pct;
    }
    else {
        mon_p->frag_pct = 0; /*no fragmentation if all the RAM is used*/
    }

    mon_p->max_used = max_used;

//    DEBUG_LOG("finished");
}
#endif

/**
 * Same as `memset` but optimized for 4 byte operation.
 * @param dst pointer to the destination buffer
 * @param v value to set [0..255]
 * @param len number of byte to set
 */
void pbl_memset(void *dst, uint8_t v, size_t len) {

	uint8_t *d8 = (uint8_t *)dst;

	uintptr_t d_align = (pbl_uintptr_t) d8 & ALIGN_MASK;

	/*Make the address aligned*/
	if (d_align) {
		d_align = ALIGN_MASK + 1 - d_align;
		while (d_align && len) {
			SET8(v);
			len--;
			d_align--;
		}
	}

	uint32_t v32 = (uint32_t)v + ((uint32_t)v << 8) + ((uint32_t)v << 16) + ((uint32_t)v << 24);

	uint32_t *d32 = (uint32_t *)d8;

	while (len > 32) {
		REPEAT8(SET32(v32));
		len -= 32;
	}

	while (len > 4) {
		SET32(v32);
		len -= 4;
	}

	d8 = (uint8_t *)d32;
	while (len) {
		SET8(v);
		len--;
	}
}

/**
 * Same as `memcpy` but optimized for 4 byte operation.
 * @param dst pointer to the destination buffer
 * @param src pointer to the source buffer
 * @param len number of byte to copy
 */
void *pbl_memcpy(void *dst, const void *src, size_t len) {
	uint8_t *d8 = dst;
	const uint8_t *s8 = src;

	pbl_uintptr_t d_align = (pbl_uintptr_t)d8 & ALIGN_MASK;
	pbl_uintptr_t s_align = (pbl_uintptr_t)s8 & ALIGN_MASK;

	/*Byte copy for unaligned memories*/
	if (s_align != d_align) {
		while (len > 32) {
			REPEAT8(COPY8);
			REPEAT8(COPY8);
			REPEAT8(COPY8);
			REPEAT8(COPY8);
			len -= 32;
		}
		while (len) {
			COPY8
			len--;
		}
		return dst;
	}

	/*Make the memories aligned*/
	if (d_align) {
		d_align = ALIGN_MASK + 1 - d_align;
		while (d_align && len) {
			COPY8;
			d_align--;
			len--;
		}
	}

	uint32_t *d32 = (uint32_t *)d8;
	const uint32_t *s32 = (uint32_t *)s8;
	while (len > 32) {
		REPEAT8(COPY32)
		len -= 32;
	}

	while (len > 4) {
		COPY32;
		len -= 4;
	}

	d8 = (uint8_t *)d32;
	s8 = (const uint8_t *)s32;
	while (len) {
		COPY8
		len--;
	}

	return dst;
}

#if PBL_MEM_MONITOR
#define LV_UNUSED(x) ((void)x)

/**********************
 *   STATIC FUNCTIONS
 **********************/
static void pbl_mem_walker(void * ptr, size_t size, int used, void * user)
{
    LV_UNUSED(ptr);

    pbl_mem_monitor_t * mon_p = user;
    if(used) {
        mon_p->used_cnt++;
    }
    else {
        mon_p->free_cnt++;
        mon_p->free_size += size;
        if(size > mon_p->free_biggest_size)
            mon_p->free_biggest_size = size;
    }
}
#endif
