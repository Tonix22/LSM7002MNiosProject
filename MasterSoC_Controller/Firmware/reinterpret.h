#pragma once
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void     u32_to_char(uint32_t w, char out);
float    u32_to_float(uint32_t w);
int32_t  u32_to_int32(uint32_t w);
double   two_u32_to_double(uint32_t w_high, uint32_t w_low);
double*  two_u32_to_double_ptr(uint32_t w_high, uint32_t w_low);

#ifdef __cplusplus
}
#endif
