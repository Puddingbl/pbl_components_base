#include "pbl_tick.h"


static uint32_t pbl_sys_time = 0;
static volatile uint8_t pbl_tick_irq_flag;


/**
 * You have to call this function periodically
 * @param tick_period the call period of this function in milliseconds
 */
void pbl_tick_inc(uint32_t tick_period) {
    pbl_tick_irq_flag = 0;
    pbl_sys_time += tick_period;
}

/**
 * Get the elapsed milliseconds since start up
 * @return the elapsed milliseconds
 */
uint32_t pbl_tick_get(void) {
    uint32_t result;
    do {
        pbl_tick_irq_flag = 1;
        result        = pbl_sys_time;
    } while(!pbl_tick_irq_flag); /*Continue until see a non interrupted cycle*/

    return result;
}

/**
 * Get the elapsed milliseconds since a previous time stamp
 * @param 上一次的时钟值
 * @return the elapsed milliseconds since 'prev_tick'
 */
uint32_t pbl_tick_elaps(uint32_t prev_tick) {
    uint32_t act_time = pbl_tick_get();

    /*If there is no overflow in sys_time simple subtract*/
    if(act_time >= prev_tick) {
        prev_tick = act_time - prev_tick;
    }
    else {
        prev_tick = UINT32_MAX - prev_tick + 1;
        prev_tick += act_time;
    }

    return prev_tick;
}
