#include "Calibrations.h"
#include "mcu_program_lms7_dc_iq_calibration_bin.h"

void CalibrateAll(LMS7002M_t *self, const LMS7002M_chan_t channel, double bw)
{
    bool useExtLoopback = false;
    int status = 0;
    // if(rgrCalibrationMethod->GetSelection() == 0)
    //     useExtLoopback = false;
    // else
    //     useExtLoopback = true;
   // double bandwidth_MHz = 0;
   // txtCalibrationBW->GetValue().ToDouble(&bandwidth_MHz);

   if (channel == LMS_CHAB)
   { 
    status = Calibrate(self, LMS_TX, LMS_CHA, bw, useExtLoopback);
    status |= Calibrate(self, LMS_TX, LMS_CHB, bw, useExtLoopback);
      if (status != 0)
      {
         return;
      }
   } else status |= Calibrate(self, LMS_TX, channel, bw, useExtLoopback);

    status |= Calibrate(self, LMS_RX, channel, bw, useExtLoopback);

   if (status != 0)
      {
         return;
      }
}

int Calibrate(LMS7002M_t *self, LMS7002M_dir_t direction, LMS7002M_chan_t channel, double bw, unsigned flags)
{
    //lime::LMS7002M* lms = SelectChannel(channel);
    int ret;
    uint16_t reg20;
    uint16_t mask;

    if (channel == LMS_CHA)
        mask = 0x28;
    else if (channel == LMS_CHB)
        mask = 0x14;
    else
        return -1;

    reg20 = LMS7002M_spi_read(self, 0x20);
    LMS7002M_spi_write(self, 0x20, mask);

    if (direction == LMS_TX)
        ret = CalibrateTx(self, channel, bw, flags & 1);
    else
        ret = CalibrateRx(self, channel, bw, flags & 1);
    LMS7002M_spi_write(self, 0x20, reg20);
    return ret; 
}


int CalibrateTx(LMS7002M_t *self, LMS7002M_chan_t channel, double bw, bool useExtLoopback)
{
    if (TrxCalib_RF_LimitLow > bw)
    {
        printf("Calibrating Tx for %g MHz (requested %g MHz [out of range])", TrxCalib_RF_LimitLow/1e6, bw/1e6);
        bw = TrxCalib_RF_LimitLow;
    }
    else if (bw > TrxCalib_RF_LimitHigh)
    {
        printf("Calibrating Tx for %g MHz (requested %g MHz [out of range])", TrxCalib_RF_LimitHigh/1e6, bw/1e6);
        bw = TrxCalib_RF_LimitHigh;
    }

// #ifdef LMS_VERBOSE_OUTPUT
//     auto beginTime = std::chrono::high_resolution_clock::now();
// #endif
    int status;
   //  uint8_t ch = (uint8_t)Get_SPI_Reg_bits(LMS7_MAC);
   //  if(ch == 0 || ch == 3)
   //      return ReportError(EINVAL, "Tx Calibration: Incorrect channel selection MAC %i", ch);

    //caching variables
    //DeviceInfo info = controlPort->GetDeviceInfo();
    //double txFreq = GetFrequencySX(LMS7002M::Tx);
    //uint8_t channel = ch == 1 ? 0 : 1;
    //int band = Get_SPI_Reg_bits(LMS7_SEL_BAND1_TRF) ? 0 : 1;

    //int dccorri(0), dccorrq(0), gcorri(0), gcorrq(0),phaseOffset(0);

    if(ReadMCUProgramID(self) != MCU_ID_CALIBRATIONS_SINGLE_IMAGE)
    {
        if((status = Program_MCU(self, mcu_program_lms7_dc_iq_calibration_bin, SRAM, 1024*16)))
            return -1;
    }

    //set reference clock parameter inside MCU
    long refClk =  self->cgen_fref; //GetReferenceClk_SX(false);
   // mcuControl->SetParameter(MCU_BD::MCU_REF_CLK, refClk);
    SetParameter(self, MCU_REF_CLK, refClk);
   // verbose_printf("MCU Ref. clock: %g MHz\n", refClk / 1e6);
    //Tx Rx separation bandwidth while calibrating
   // mcuControl->SetParameter(MCU_BD::MCU_BW, bandwidth_Hz);
    SetParameter(self, MCU_BW, bw);

    {
       //BoardLoopbackStore onBoardLoopbackRestoration(GetConnection());
        if(useExtLoopback)
        {
            // status = SetExtLoopback(controlPort, ch, true, true);
            // if(status != 0)
            //     return ReportError(EINVAL, "Tx Calibration: Failed to enable external loopback");
            // uint8_t loopPair = GetExtLoopPair(*this, true);
            // mcuControl->SetParameter(MCU_BD::MCU_EXT_LOOPBACK_PAIR, loopPair);
        }
        RunProcedure(self, useExtLoopback ? MCU_FUNCTION_CALIBRATE_TX_EXTLOOPB : MCU_FUNCTION_CALIBRATE_TX);
        status = WaitForMCU(self, 1000);
        if(status != 0)
            return -1;//ReportError(EINVAL, "Tx Calibration: MCU error %i (%s)", status, MCU_BD::MCUStatusMessage(status));
    }

    //sync registers to cache
   //  const std::vector<uint16_t> regsToSync = {0x0208, 0x05C0};
   //  for(const auto addr : regsToSync)
   //      this->SPI_read(addr, true);
    LMS7002M_regs_spi_read(self, 0x0208);
    LMS7002M_regs_spi_read(self, 0x05C0);

    return 0;
}

int CalibrateRx(LMS7002M_t *self, LMS7002M_chan_t channel, double bw, bool useExtLoopback)
{
    if (TrxCalib_RF_LimitLow > bw)
    {
        printf("Calibrating Rx for %g MHz (requested %g MHz [out of range])", TrxCalib_RF_LimitLow/1e6, bw/1e6);
        bw = TrxCalib_RF_LimitLow;
    }
    else if (bw > TrxCalib_RF_LimitHigh)
    {
        printf("Calibrating Rx for %g MHz (requested %g MHz [out of range])", TrxCalib_RF_LimitHigh/1e6, bw/1e6);
        bw = TrxCalib_RF_LimitHigh;
    }

    int status;
   //  uint8_t ch = (uint8_t)Get_SPI_Reg_bits(LMS7_MAC);
   //  if(ch == 0 || ch == 3)
   //      return ReportError(EINVAL, "Rx Calibration: Incorrect channel selection MAC %i", ch);
   //  uint8_t channel = ch == 1 ? 0 : 1;
   // uint8_t lna = (uint8_t)Get_SPI_Reg_bits(self, 0x010D, 8, 7);  //(LMS7_SEL_PATH_RFE);
    //double rxFreq = GetFrequencySX(LMS7002M::Rx);

   // int dcoffi(0), dcoffq(0), gcorri(0), gcorrq(0), phaseOffset(0);
    //check if MCU has correct firmware
     if(ReadMCUProgramID(self) != MCU_ID_CALIBRATIONS_SINGLE_IMAGE)
    {
        if((status = Program_MCU(self, mcu_program_lms7_dc_iq_calibration_bin, SRAM, 1024*16)))
            return -1;
    }

    //set reference clock parameter inside MCU
    long refClk =  self->cgen_fref; //GetReferenceClk_SX(false);
   // mcuControl->SetParameter(MCU_BD::MCU_REF_CLK, refClk);
    SetParameter(self, MCU_REF_CLK, refClk);
   // verbose_printf("MCU Ref. clock: %g MHz\n", refClk / 1e6);
    //Tx Rx separation bandwidth while calibrating
   // mcuControl->SetParameter(MCU_BD::MCU_BW, bandwidth_Hz);
    SetParameter(self, MCU_BW, bw);

    {
        //BoardLoopbackStore onBoardLoopbackRestoration(GetConnection());
        if(useExtLoopback)
        {
            // status = SetExtLoopback(controlPort, ch, true, false);
            // if(status != 0)
            //     return ReportError(EINVAL, "Rx Calibration: Failed to enable external loopback");
            // uint8_t loopPair = GetExtLoopPair(*this, false);
            // mcuControl->SetParameter(MCU_BD::MCU_EXT_LOOPBACK_PAIR, loopPair);
        }

        RunProcedure(self, useExtLoopback ? MCU_FUNCTION_CALIBRATE_RX_EXTLOOPB : MCU_FUNCTION_CALIBRATE_RX);
        status = WaitForMCU(self, 1000);
        if(status != 0)
            return -1; //ReportError(EINVAL, "Rx calibration: MCU error %i (%s)", status, MCU_BD::MCUStatusMessage(status));
    }

    //sync registers to cache
   //  const std::vector<uint16_t> regsToSync = {0x040C, 0x05C0};
   //  for(const auto addr : regsToSync)
   //      this->SPI_read(addr, true);

      LMS7002M_regs_spi_read(self, 0x040C);
      LMS7002M_regs_spi_read(self, 0x05C0);

    return 0;
}


