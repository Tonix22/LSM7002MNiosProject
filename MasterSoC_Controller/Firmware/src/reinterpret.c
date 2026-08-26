#include "reinterpret.h" 

double u32_to_double(uint32_t w_high, uint32_t w_low) {
    uint64_t u = w_high;
    u = (((u) << 32) & 0xFFFFFFFF00000000) | (w_low & 0x00000000FFFFFFFF);
    double d;
    memcpy(&d, &u, sizeof(double)); 
    return d;
}


void u32_to_double_ptr(uint32_t w_high, uint32_t w_low, double* out) {
    uint64_t u = w_high;
    u = (((u) << 32) & 0xFFFFFFFF00000000) | (w_low & 0x00000000FFFFFFFF);
    memcpy(out, &u, sizeof(double));
}


float u32_to_float(uint32_t w){
    float f;
    memcpy(&f, &w, sizeof(float));
    return f;
}

int32_t u32_to_int32 (uint32_t w){
    int32_t i;
    memcpy(&i, &w, sizeof(int32_t));
    return i;
}

char u32_to_char(uint32_t w){
    char c;
    memcpy(&c, &w, sizeof(char));
    return c;
}

short u32_to_short(uint32_t w)
{
    uint16_t low = (uint16_t)(w & 0xFFFF);

    if (low & 0x8000)
        return (short)((int32_t)low - 0x10000);

    return (short)low;
}