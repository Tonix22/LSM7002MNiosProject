
#ifndef TUNE_FILTER_8051_H
#define TUNE_FILTER_8051_H

// #include <stdio.h>

// #include <string.h>
// #include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include "LMS7002M_impl.h"
#include <LMS7002M/LMS7002M_time.h>

#define MCU_ID_CALIBRATIONS_SINGLE_IMAGE 0x05
#define SRAM 2
#define AGC_K_RXTSP 0x0408, 17<<4 |  0
#define AGC_ADESIRED_RXTSP 0x0409, 15<<4 |  4
#define AGC_MODE_RXTSP 0x040A, 13<<4 |  12
#define AGC_AVG_RXTSP 0x040A, 2<<4 |  0

typedef double float_type;

typedef enum {
    MCU_REF_CLK,
    MCU_BW,
    MCU_EXT_LOOPBACK_PAIR
} MCU_Parameter;

// #ifdef __cplusplus
// extern "C" {
// #endif


//rx lpf range limits
static const float_type RxLPF_RF_LimitLow = 1.4e6;
static const float_type RxLPF_RF_LimitHigh = 130e6;

//tx lpf range limits
static const float_type TxLPF_RF_LimitLow = 5e6;
static const float_type TxLPF_RF_LimitLowMid = 40e6;
static const float_type TxLPF_RF_LimitMidHigh = 50e6;
static const float_type TxLPF_RF_LimitHigh = 130e6;

int TuneTxFilter_8051(LMS7002M_t *self, const LMS7002M_chan_t channel, float_type tx_lpf_freq_RF, double *bwactual);
uint8_t ReadMCUProgramID(LMS7002M_t *self);
void RunProcedure(LMS7002M_t *self, uint8_t id);
int WaitForMCU( LMS7002M_t *self, uint32_t timeout_ms);
int Program_MCU( LMS7002M_t *self, const uint8_t* buffer, const uint8_t mode, uint32_t byte_array_size);
void SetParameter( LMS7002M_t *self, MCU_Parameter param, float value);
float_type GetTBBIAMP_dB(LMS7002M_t *self, const LMS7002M_chan_t channel, int ind);
int SetTBBIAMP_dB(LMS7002M_t *self, const LMS7002M_chan_t channel, const float_type gain, int ind);
int CalibrateTxGain(LMS7002M_t *self, const LMS7002M_chan_t channel, int ind, float maxGainOffset_dBFS, float *actualGain_dBFS);
int CalibrateTxGainSetup(LMS7002M_t *self, LMS7002M_chan_t channel);
float_type GetFrequencyCGEN(LMS7002M_t *self);
float_type GetReferenceClk_TSP(LMS7002M_t *self, bool tx);
uint16_t Get_SPI_Reg_bits(LMS7002M_t *self, uint16_t address, uint8_t msb, uint8_t lsb);
void Modify_SPI_Reg_bits(LMS7002M_t *self, const uint16_t address, const uint8_t msb, const uint8_t lsb, const uint16_t value);
static inline int LMS7002M_regs_default_tofilter(const int addr);
void set_addrs_to_defaultFilter(LMS7002M_t *self, const LMS7002M_chan_t channel, const int start_addr, const int stop_addr);
//int SetDefaults(LMS7002M_t *self, uint16_t dir1, uint16_t dir2);

// #ifdef __cplusplus
// }
// #endif

#endif