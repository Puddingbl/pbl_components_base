#ifndef __PBL_TLSF_CONF_H__
#define __PBL_TLSF_CONF_H__

#include "rtdbg.h"

typedef unsigned char uint8_t;
typedef unsigned int uint32_t;

//�����ڴ�ش�С
// �����Է��֣�Լ3212�ֽ��Ǳض����ĵģ�������С����5K�ȽϺ�
#define PBL_MEM_SIZE  (10*1024)  

#define PBL_MEM_MONITOR 0

#define PBL_DBG_LEVEL 0

#if PBL_DBG_LEVEL >= 3
#define PBL_MEM_LOG_D    LOG_D
#else
#define PBL_MEM_LOG_D(...)
#endif

#if PBL_DBG_LEVEL >= 2
#define PBL_MEM_LOG_I     LOG_I
#else
#define PBL_MEM_LOG_I(...)
#endif

#if PBL_DBG_LEVEL >= 1
#define PBL_MEM_LOG_W     LOG_W
#else
#define PBL_MEM_LOG_W(...)
#endif

#if PBL_DBG_LEVEL >= 0
#define PBL_MEM_LOG_E     LOG_E
#else
#define PBL_MEM_LOG_E(...)
#endif

#endif
