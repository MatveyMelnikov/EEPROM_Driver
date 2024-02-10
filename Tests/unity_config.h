#ifndef UNITY_CONFIG_H
#define UNITY_CONFIG_H

#include <stdint.h>

extern void unity_config_put_c(uint8_t a);

#define UNITY_OUTPUT_CHAR(a) \
    unity_config_put_c(a)
#define UNITY_PRINT_EOL() \
    unity_config_put_c('\r'); \
    unity_config_put_c('\n')

/*
// Add float(?)
#define UNITY_INT_WIDTH 32
#define UNITY_POINTER_WIDTH 32
#define CMOCK_MEM_INDEX_TYPE uint32_t
#define CMOCK_MEM_PTR_AS_INT uint32_t
#define CMOCK_MEM_ALIGN 1
#define CMOCK_MEM_SIZE 20480 // 20 kb in stm32 of sram (?)
*/

#include "unity.h"

#endif
