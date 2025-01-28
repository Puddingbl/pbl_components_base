/* define to prevent recursive inclusion -------------------------------------*/
#ifndef __PBL_TICK_H
#define __PBL_TICK_H



/* includes ------------------------------------------------------------------*/
#include "at32f425.h"


#ifdef __cplusplus
extern "C" {
#endif
/* define---------------------------------------------------------------------*/



/* exported functions ------------------------------------------------------- */
void pbl_tick_inc(uint32_t tick_period);
uint32_t pbl_tick_get(void);
uint32_t pbl_tick_elaps(uint32_t prev_tick);


/* extern variables-----------------------------------------------------------*/


#ifdef __cplusplus
}
#endif

#endif


