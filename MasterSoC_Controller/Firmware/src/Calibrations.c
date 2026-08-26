#include "Calibrations.h"
#include "mcu_program_lms7_dc_iq_calibration_bin.h"
#include "LMS7002M_enable_external_loopback.h"

void CalibrateAll(LMS7002M_t *self, const LMS7002M_chan_t channel, double bw)
{
    bool useExtLoopback = 0;
    int status = 0;

   if (channel == LMS_CHAB)
   { 
    status = Calibrate(self, LMS_TX, LMS_CHA, bw, useExtLoopback);
    status |= Calibrate(self, LMS_TX, LMS_CHB, bw, useExtLoopback);
    status |= Calibrate(self, LMS_RX, LMS_CHA, bw, useExtLoopback);
    status |= Calibrate(self, LMS_RX, LMS_CHB, bw, useExtLoopback);
      if (status != 0)
      {
         return;
      }
   } else status = Calibrate(self, LMS_TX, channel, bw, useExtLoopback);

    status |= Calibrate(self, LMS_RX, channel, bw, useExtLoopback);

}

void CalibrateTx(LMS7002M_t *self, LMS7002M_chan_t channel, double bw)
{
     if (channel == LMS_CHAB) { 
        Calibrate(self, LMS_TX, LMS_CHA, bw, 0);
        Calibrate(self, LMS_TX, LMS_CHB, bw, 0);
    } else Calibrate(self, LMS_TX, channel, bw, 0);

}

void CalibrateRx(LMS7002M_t *self, LMS7002M_chan_t channel, double bw)
{
    if (channel == LMS_CHAB) { 
        Calibrate(self, LMS_RX, LMS_CHA, bw, 0);
        Calibrate(self, LMS_RX, LMS_CHB, bw, 0);
    } else Calibrate(self, LMS_RX, channel, bw, 0);
}

int Calibrate(LMS7002M_t *self, LMS7002M_dir_t direction, LMS7002M_chan_t channel, double bw, unsigned flags)
{
    //lime::LMS7002M* lms = SelectChannel(channel);
    int ret;
    uint16_t reg20;
    uint16_t mask;

    if (channel == LMS_CHA)
        mask = 0x14;
    else if (channel == LMS_CHB)
        mask = 0x28;
    else
        return -1;

    reg20 = LMS7002M_spi_read(self, 0x20);
    LMS7002M_spi_write(self, 0x20, reg20 | mask);

    if (direction == LMS_TX)
        ret = Calibrate_Tx(self, channel, bw, flags & 1);
    else
        ret = Calibrate_Rx(self, channel, bw, flags & 1);
    LMS7002M_spi_write(self, 0x20, reg20);
    return ret; 
}


int Calibrate_Tx(LMS7002M_t *self, LMS7002M_chan_t channel, double bw, bool useExtLoopback)
{
    if (TrxCalib_RF_LimitLow > bw)
    {
        bw = TrxCalib_RF_LimitLow;
    }
    else if (bw > TrxCalib_RF_LimitHigh)
    {
        bw = TrxCalib_RF_LimitHigh;
    }

    int status;

    if(ReadMCUProgramID(self) != MCU_ID_CALIBRATIONS_SINGLE_IMAGE)
    {
        if((status = Program_MCU(self, mcu_program_lms7_dc_iq_calibration_bin, SRAM, 1024*16)))
            return -1;
    }

    //set reference clock parameter inside MCU
    long refClk =  self->cgen_fref; //GetReferenceClk_SX(false);
   
    SetParameter(self, MCU_REF_CLK, refClk);
   
    SetParameter(self, MCU_BW, bw);

    {
       //BoardLoopbackStore onBoardLoopbackRestoration(GetConnection());
        if(useExtLoopback)
        {
            LMS7002M_enable_external_loopback(self, channel, 1);
           
            uint8_t loopPair = 1 << 2 | 0x1;//GetExtLoopPair(*this, true);
            SetParameter(self, MCU_EXT_LOOPBACK_PAIR, loopPair);
        }
        RunProcedure(self, useExtLoopback ? MCU_FUNCTION_CALIBRATE_TX_EXTLOOPB : MCU_FUNCTION_CALIBRATE_TX);
        status = WaitForMCU(self, 1000);
    }    

    LMS7002M_regs_spi_read(self, 0x0208);
    LMS7002M_regs_spi_read(self, 0x05C0);
    LMS7002M_enable_external_loopback(self, channel, 0);
    
    //need to read back calibration results
    int ch;
    if (channel == LMS_CHA)
        ch = 0;
    else if(channel == LMS_CHB)
        ch = 1;
    else 
        ch = 0; //default to channel A

    int16_t dccorri = ReadAnalogDC(self, ch ? 0x05C5 : 0x05C3);
    int16_t dccorrq = ReadAnalogDC(self, ch ? 0x05C6 : 0x05C4);

    return 0;
}

int Calibrate_Rx(LMS7002M_t *self, LMS7002M_chan_t channel, double bw, bool useExtLoopback)
{
    if (TrxCalib_RF_LimitLow > bw)
    {
        bw = TrxCalib_RF_LimitLow;
    }
    else if (bw > TrxCalib_RF_LimitHigh)
    {
        bw = TrxCalib_RF_LimitHigh;
    }

    int status;
     if(ReadMCUProgramID(self) != MCU_ID_CALIBRATIONS_SINGLE_IMAGE)
    {
        if((status = Program_MCU(self, mcu_program_lms7_dc_iq_calibration_bin, SRAM, 1024*16)))
            return -1;
    }

    //set reference clock parameter inside MCU
    long refClk =  self->cgen_fref; 
   
    SetParameter(self, MCU_REF_CLK, refClk);
   
    SetParameter(self, MCU_BW, bw);

    {
        //BoardLoopbackStore onBoardLoopbackRestoration(GetConnection());
        if(useExtLoopback)
        {
             LMS7002M_enable_external_loopback(self, channel, 1);
             uint8_t loopPair = 1 << 2 | 0x1; //GetExtLoopPair(*this, false);
             SetParameter(self, MCU_EXT_LOOPBACK_PAIR, loopPair);
        }

        RunProcedure(self, useExtLoopback ? MCU_FUNCTION_CALIBRATE_RX_EXTLOOPB : MCU_FUNCTION_CALIBRATE_RX);
        status = WaitForMCU(self, 1000);
    }

      LMS7002M_regs_spi_read(self, 0x040C);
      LMS7002M_regs_spi_read(self, 0x05C0);
      LMS7002M_enable_external_loopback(self, channel, 0);
      
    
    int ch;
    if (channel == LMS_CHA)
        ch = 0;
    else if(channel == LMS_CHB)
        ch = 1;
    else 
        ch = 0; //default to channel A
    int16_t dcoffi = ReadAnalogDC(self, ch ? 0x05C9 : 0x05C7);
    int16_t dcoffq = ReadAnalogDC(self, ch ? 0x05CA : 0x05C8);

    return 0;
}


int16_t ReadAnalogDC(LMS7002M_t *self, uint16_t address)
{
   uint16_t mask = address < 0x05C7 ? 0x03FF : 0x003F;

    LMS7002M_spi_write(self, address, 0);
    LMS7002M_spi_write(self, address, 0x4000);
    uint16_t value = LMS7002M_spi_read(self, address);
    LMS7002M_spi_write(self, address, value & ~0xC000);
    int16_t result = (value & mask);
    if(value& (mask+1))
        result *= -1;
    return result;
}


float_type GetFrequencySX(LMS7002M_t *self, const LMS7002M_chan_t channel, const bool Tx)
{
    Modify_SPI_Reg_bits(self, 0x0020, 1, 0, Tx ? 2 : 1); // Rx mac = 1, Tx mac = 2
    {
        const uint16_t gINT = Get_SPI_Reg_bits(self, 0x011E, 13, 0);    // read whole register to reduce SPI transfers
        const uint32_t gFRAC = ((uint32_t)(gINT&0xF) << 16) | LMS7002M_spi_read(self, 0x011D);
        const uint8_t enDiv2 = Get_SPI_Reg_bits(self, 0x011C, 10, 10)+1;
        const uint8_t divLoch = Get_SPI_Reg_bits(self, 0x011F, 8, 6) + 1;
        LMS7002M_spi_write(self, 0x0020, channel);
        //Calculate real frequency according to the calculated parameters
        return (enDiv2) * (self->cgen_fref / (1 << divLoch)) * ((gINT >> 4) + 4 + (gFRAC / 1048576.0));
    }
}

