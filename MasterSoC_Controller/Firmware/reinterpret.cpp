
#include <cstdint>
//#include <cstring>
#include <cstdio>

extern "C" {


void u32_to_char(uint32_t w, char& out) {
    const char* p = reinterpret_cast<const char*>(&w);
   // out[0]=p[0]; 
    //out[1]=p[1]; 
    //out[2]=p[2]; 
    //out[3]=p[3];
    out = p[3];
}

float u32_to_float(uint32_t w) {
    return *reinterpret_cast<float*>(&w);
}

int32_t u32_to_int32 (uint32_t w){
    return *reinterpret_cast<int32_t*>(&w);
}

double two_u32_to_double(uint32_t w_high, uint32_t w_low) {
    uint64_t u = w_high & 0x00000000FFFFFFFF;
    printf(" Valor de w_high en two_u32_to_double: %lx\n", u);
    u = ((((u) << 32) & 0xFFFFFFFF00000000) | (w_low & 0x00000000FFFFFFFF));
    printf(" Valor de u en two_u32_to_double: %lx\n", u);
    return *reinterpret_cast<double*>(&u);
} 
double* two_u32_to_double_ptr(uint32_t w_high, uint32_t w_low) {
    uint64_t u = w_high;
    u = (((u) << 32) & 0xFFFFFFFF00000000) | (w_low & 0x00000000FFFFFFFF);
    return reinterpret_cast<double*>(u);
} 
} // extern "C"