
#include <stdint.h>

double u32_to_double(uint32_t w_high, uint32_t w_low);
int32_t u32_to_int32 (uint32_t w);
float u32_to_float(uint32_t w);
char u32_to_char(uint32_t w);
void u32_to_double_ptr(uint32_t w_high, uint32_t w_low, double* out);