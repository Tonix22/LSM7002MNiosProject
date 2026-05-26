#include "Tune_Filter_8051.h"

#define MCU_FUNCTION_CALIBRATE_TX 1
#define MCU_FUNCTION_CALIBRATE_RX 2
#define MCU_FUNCTION_CALIBRATE_TX_EXTLOOPB 17
#define MCU_FUNCTION_CALIBRATE_RX_EXTLOOPB 18

static const double TrxCalib_RF_LimitLow = 2.5e6;
static const double TrxCalib_RF_LimitHigh = 120e6;

void CalibrateAll(LMS7002M_t *self, const LMS7002M_chan_t channel, double bw);
int Calibrate(LMS7002M_t *self, LMS7002M_dir_t direction, LMS7002M_chan_t channel, double bw, unsigned flags);
int CalibrateTx(LMS7002M_t *self, LMS7002M_chan_t channel, double bw, bool useExtLoopback);
int CalibrateRx(LMS7002M_t *self, LMS7002M_chan_t channel, double bw, bool useExtLoopback);