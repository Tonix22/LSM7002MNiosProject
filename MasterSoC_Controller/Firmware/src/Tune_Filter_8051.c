
#include "Tune_Filter_8051.h"
#include <stdio.h>
#include <stdbool.h>
#include "mcu_program_lms7_dc_iq_calibration_bin.h"

static inline int LMS7002M_regs_default_tofilter(const int addr);


int TuneRxFilter_8051(LMS7002M_t *self, const LMS7002M_chan_t channel, float_type rx_lpf_freq_RF, double *bwactual)
{
    int status;

    LMS7002M_set_mac_ch(self, channel);
    
    if (rx_lpf_freq_RF < RxLPF_RF_LimitLow) {
        rx_lpf_freq_RF = RxLPF_RF_LimitLow;
    }
    if (rx_lpf_freq_RF > RxLPF_RF_LimitHigh) {
        rx_lpf_freq_RF = RxLPF_RF_LimitHigh;
    }

    
    uint8_t g_tia = Get_SPI_Reg_bits(self, 0x0113, 1, 0);

    if(g_tia == 1 && rx_lpf_freq_RF < 4e6)
    {
        rx_lpf_freq_RF = 4e6;
       // Log(LOG_WARNING, "Rx LPF min bandwidth is 4MHz when TIA gain is set to -12 dB");
    }


     if(ReadMCUProgramID(self) != MCU_ID_CALIBRATIONS_SINGLE_IMAGE)
    {
        if((status = Program_MCU(self, mcu_program_lms7_dc_iq_calibration_bin, SRAM, 1024*16)))
            return -1;
    }

    //set reference clock parameter inside MCU
    long refClk =  self->cgen_fref;   //GetReferenceClk_SX(false);
    SetParameter(self, MCU_REF_CLK, refClk);
    
    //set bandwidth for MCU to read from register, value is integer stored in MHz
    SetParameter(self, MCU_BW, rx_lpf_freq_RF);
    RunProcedure(self, 5);

    status = WaitForMCU(self, 1000);
    if(status != 0)
    {
        return -1;
    }
    //sync registers to cache
    LMS7002M_regs_spi_read(self, 0x0112);
    LMS7002M_regs_spi_read(self, 0x0117);
    LMS7002M_regs_spi_read(self, 0x011A);
    LMS7002M_regs_spi_read(self, 0x0116);
    LMS7002M_regs_spi_read(self, 0x0118);
    LMS7002M_regs_spi_read(self, 0x0114);
    LMS7002M_regs_spi_read(self, 0x0019);
    LMS7002M_regs_spi_read(self, 0x0115);    

    return status;
}





static int opt_gain_tbb[2] = {-1, -1};


int TuneTxFilter_8051(LMS7002M_t *self, const LMS7002M_chan_t channel, float_type tx_lpf_freq_RF, double *bwactual)
{
    int status;

    int ind ; //GetActiveChannelIndex()%2;
    if (channel == LMS_CHA)
        ind = 0;
    else if(channel == LMS_CHB)
        ind = 1;
    else 
        ind = 0; //default to channel A

    LMS7002M_set_mac_ch(self, channel);
    
    if (tx_lpf_freq_RF < TxLPF_RF_LimitLow) {
        tx_lpf_freq_RF = TxLPF_RF_LimitLow;
    }
    if (tx_lpf_freq_RF > TxLPF_RF_LimitHigh) {
        tx_lpf_freq_RF = TxLPF_RF_LimitHigh;
    }

    int gain = GetTBBIAMP_dB(self, channel, ind);

    //calculate intermediate frequency
    float_type tx_lpf_IF = tx_lpf_freq_RF/2;

    if(tx_lpf_freq_RF > TxLPF_RF_LimitLowMid && tx_lpf_freq_RF < TxLPF_RF_LimitMidHigh)
    {
        tx_lpf_IF = TxLPF_RF_LimitMidHigh/2;
    }

    if(ReadMCUProgramID(self) != MCU_ID_CALIBRATIONS_SINGLE_IMAGE)
    {
        if((status = Program_MCU(self, mcu_program_lms7_dc_iq_calibration_bin, SRAM, 1024*16)))
            return -1;
    }
    
     opt_gain_tbb[ind] = -1;

    //set reference clock parameter inside MCU
    long refClk =  self->cgen_fref;   

    SetParameter(self, MCU_REF_CLK, refClk);

    //set bandwidth for MCU to read from register, value is integer stored in MHz
    SetParameter(self, MCU_BW, tx_lpf_freq_RF);
    RunProcedure(self, 6);
    status = WaitForMCU(self, 1000);
    
    if(status != 0)
    {
        return -1;
    }
    //sync registers to cache
    LMS7002M_regs_spi_read(self, 0x0105);
    LMS7002M_regs_spi_read(self, 0x0106);
    LMS7002M_regs_spi_read(self, 0x0109);
    LMS7002M_regs_spi_read(self, 0x010A);
    LMS7002M_regs_spi_read(self, 0x010B);

    SetTBBIAMP_dB(self, channel, gain, ind);

    return 0;
}

void RunProcedure(LMS7002M_t*self, uint8_t id)
{
    LMS7002M_spi_write(self, 0x0006, 1);
    LMS7002M_spi_write(self, 0x0000, id);
    uint8_t x0002reg = LMS7002M_spi_read(self, 0x0002);
    const uint8_t interupt6 = 0x08;
    LMS7002M_spi_write(self, 0x0002, x0002reg & ~interupt6);
    LMS7002M_spi_write(self, 0x0002, x0002reg | interupt6);
    LMS7002M_spi_write(self, 0x0002, x0002reg & ~interupt6);
    //MCU seems to be stuck at this point until any SPI operation is performed
    LMS7002M_spi_read(self, 0x0002); //random spi action
    LMS7_sleep_for(10); //10 us
}

int WaitForMCU(LMS7002M_t *self, uint32_t timeout_ms)
{
    long long ticks;
    long long t1 = LMS7_time_now();
    long long t2;

    unsigned short value = 0;
    LMS7_sleep_for(50); // 50us
 
    do {
        value = LMS7002M_spi_read(self, 0x0001) & 0xFF;
        if (value != 0xFF) //working
            break;  
        LMS7_sleep_for(1000 * 1);   // sleep 1 ms
        t2 = LMS7_time_now();       
    } while ((t2 - t1) < 1000*timeout_ms); 

    LMS7002M_spi_write(self, 0x0006, 0); //return SPI control to PC

    return value & 0x7F;
}


uint8_t ReadMCUProgramID(LMS7002M_t *self)
{
    RunProcedure(self, 255);
    uint8_t statusMcu = WaitForMCU(self, 10);
    return statusMcu & 0x7F;
}


int Program_MCU( LMS7002M_t *self, const uint8_t* buffer, const uint8_t mode, uint32_t byte_array_size)
{
    //const auto timeout = std::chrono::milliseconds(100);
    long long timeout = 100; 
    const uint32_t controlAddr = 0x0002 << 16;
    const uint32_t statusReg = 0x0003 << 16;
    const uint32_t addrDTM = 0x0004 << 16; //data to MCU
    const uint16_t EMTPY_WRITE_BUFF = 1 << 0;
    const uint16_t PROGRAMMED = 1 << 6;
    const uint8_t fifoLen = 64;
    uint32_t wrdata[fifoLen];
    uint32_t rddata = 0;
    int status;
    bool abort = false;
        //reset MCU, set mode
    wrdata[0] = (1 << 31) | controlAddr | 0;
    wrdata[1] = (1 << 31) | controlAddr | (mode & 0x3);
   
    LMS7002M_spi_write(self, (uint16_t)((wrdata[0] >> 16) & 0xFFFF), (uint16_t)(wrdata[0] & 0xFFFF));
    LMS7002M_spi_write(self, (uint16_t)((wrdata[1] >> 16) & 0xFFFF), (uint16_t)(wrdata[1] & 0xFFFF));

    long long t1;
    long long t2;
    for(uint16_t i=0; i<byte_array_size; i+=fifoLen)
    {
        //wait till EMPTY_WRITE_BUFF = 1
        bool fifoEmpty = false;
        wrdata[0] = statusReg;
        t1 = LMS7_time_now();
        
        do{
            rddata = LMS7002M_spi_read(self, (uint16_t)((wrdata[0] >> 16) & 0xFFFF)) & 0xFFFF;

            fifoEmpty = rddata & EMTPY_WRITE_BUFF;
            t2 = LMS7_time_now();
        } while( (!fifoEmpty) && (t2-t1)<timeout);
        
        if(!fifoEmpty){
           return -1; }

        //write 32 bytes into FIFO
        for(uint8_t j=0; j<fifoLen; ++j) {
            wrdata[j] = (1 << 31) | addrDTM | buffer[i+j];
            LMS7002M_spi_write(self, (uint16_t)((wrdata[j] >> 16) & 0xFFFF), (uint16_t)(wrdata[j] & 0xFFFF));
        }
    };

    //wait until programmed flag
    wrdata[0] = statusReg;
    bool programmed = false;
    t1 = LMS7_time_now();
   
    do{
        rddata = LMS7002M_spi_read(self, (uint16_t)((wrdata[0] >> 16) & 0xFFFF)) & 0xFFFF;
        programmed = rddata & PROGRAMMED;
        t2 = LMS7_time_now();
    } while( (!programmed) && (t2-t1) < timeout);
    

    if(!programmed)
        return -1;
    return 0;
}


void SetParameter( LMS7002M_t *self, MCU_Parameter param, float value)
{
    const uint8_t x0002reg = LMS7002M_spi_read(self, 0x0002);
    const uint8_t interupt7 = 0x04;
    if(param==MCU_REF_CLK || param == MCU_BW)
    {
        uint8_t inputRegs[3];
        value /= 1e6;
        inputRegs[0] = (uint8_t)value; //frequency integer part

        uint16_t fracPart = value * 1000.0 - inputRegs[0]*1000.0;
        inputRegs[1] = (fracPart >> 8) & 0xFF;
        inputRegs[2] = fracPart & 0xFF;
        for(uint8_t i = 0; i < 3; ++i)
        {
            LMS7002M_spi_write(self, 0, inputRegs[2-i]);
            LMS7002M_spi_write(self, 0x0002, x0002reg | interupt7);
            LMS7002M_spi_write(self, 0x0002, x0002reg & ~interupt7);
            LMS7_sleep_for(5); //sleep 5 us
        }
    }
    if(param==MCU_REF_CLK)
        RunProcedure(self, 4);
    if(param == MCU_BW)
        RunProcedure(self, 3);
    if(param == MCU_EXT_LOOPBACK_PAIR)
    {
        uint8_t intVal = (int)value;
        LMS7002M_spi_write(self, 0, intVal);
        LMS7002M_spi_write(self, 0x0002, x0002reg | interupt7);
        LMS7002M_spi_write(self, 0x0002, x0002reg & ~interupt7);
        int status = WaitForMCU(self, 10);
        
        RunProcedure(self, 9);
    }
    if(WaitForMCU(self, 100) != 0)
        printf("Failed to set MCU parameter\n");
    }


int SetTBBIAMP_dB(LMS7002M_t *self, const LMS7002M_chan_t channel, const float_type gain, int ind) {

    if (opt_gain_tbb[ind] <= 0)
    {
        if (CalibrateTxGain(self, channel, ind, 0, NULL)!=0) //set optimal BB gain
            return -1;
         if (fabs(gain) < 0.2) // optimal gain = ~0dB
             return 0;
    }

    int g_iamp = (float_type)opt_gain_tbb[ind]*pow(10.0,gain/20.0)+0.4;

    Modify_SPI_Reg_bits(self, 0x0108, 15, 10, g_iamp > 63 ? 63 : g_iamp<1 ? 1 :g_iamp);

    return 0;
}

float_type GetTBBIAMP_dB(LMS7002M_t *self, const LMS7002M_chan_t channel, int ind)
{
    int g_current = Get_SPI_Reg_bits(self, 0x0108, 15, 10);

    if (opt_gain_tbb[ind] <= 0)
    {
        if (CalibrateTxGain(self, channel, ind, 0, NULL)!=0){
            return 0.0;}
        Modify_SPI_Reg_bits(self, 0x0108, 15, 10, g_current); //restore
    }
    return 20.0*log10((float_type)g_current / (float_type) opt_gain_tbb[ind]);
}



int CalibrateTxGain(LMS7002M_t *self, const LMS7002M_chan_t channel, int ind, float maxGainOffset_dBFS, float *actualGain_dBFS)
{
    int status;
    int cg_iamp;
    //auto registersBackup = BackupRegisterMap();

    LMS7002M_regs_t saved_map[2];
    LMS7002M_rfic_to_regs(self);
    memcpy(saved_map, self->_regs, sizeof(saved_map));

    status = CalibrateTxGainSetup(self, channel);
    
    if(status == 0)
    {
        cg_iamp = Get_SPI_Reg_bits(self, 0x0108, 15, 10); //CG_IAMP_TBB
        
         while (LMS7002M_rxtsp_read_rssi(self, channel) < 0x7FFF)
        {
            if(++cg_iamp > 63)
                break;
            Modify_SPI_Reg_bits(self, 0x0108, 15, 10, cg_iamp);

        }
    }
 //   RestoreRegisterMap(registersBackup);
    memcpy(self->_regs, saved_map, sizeof(saved_map));
    LMS7002M_regs_to_rfic(self);
//  LMS7002M_spi_write(self, 0x8086, 0x4101);
//  LMS7002M_spi_write(self, 0x8088, 0x04D2);
//  LMS7002M_spi_write(self, 0x8089, 0x1070);
//  LMS7002M_spi_write(self, 0x808B, 0x2152);

//   LMS7002M_spi_write(self, 0x8100, 0x3409);
//   LMS7002M_spi_write(self, 0x8105, 0x0011);
//   LMS7002M_spi_write(self, 0x8108, 0x958C);
//   LMS7002M_spi_write(self, 0x810C, 0x8865);
//   LMS7002M_spi_write(self, 0x810D, 0x011A);

//  LMS7002M_spi_write(self, 0x8118, 0x418C);
//  LMS7002M_spi_write(self, 0x8119, 0x5292);
//  LMS7002M_spi_write(self, 0x811A, 0x3001);
//  LMS7002M_spi_write(self, 0x811C, 0x8943);

//  LMS7002M_spi_write(self, 0x8200, 0x00E1);
//  LMS7002M_spi_write(self, 0x8208, 0x017B);
//  LMS7002M_spi_write(self, 0x820B, 0x4000);
//  LMS7002M_spi_write(self, 0x820C, 0x8000);
//  LMS7002M_spi_write(self, 0x8242, 0x0000);
//  LMS7002M_spi_write(self, 0x8243, 0x0000);
//  LMS7002M_spi_write(self, 0x8403, 0x0000);
//  LMS7002M_spi_write(self, 0x8404, 0x0006);
//  LMS7002M_spi_write(self, 0x840A, 0x1000);
//  LMS7002M_spi_write(self, 0x840B, 0x1020);
//  LMS7002M_spi_write(self, 0x840C, 0x00FB);
//  LMS7002M_spi_write(self, 0x840E, 0x0000);
//  LMS7002M_spi_write(self, 0x840F, 0x0000);

    LMS7002M_set_mac_ch(self, channel); 
    
    opt_gain_tbb[ind] = cg_iamp > 1 ? cg_iamp-1 : 1;

    if (status == 0)
        Modify_SPI_Reg_bits(self, 0x0108, 15, 10, opt_gain_tbb[ind]);
    //logic reset
    Modify_SPI_Reg_bits(self, 0x0020, 13, 13, 0);
    Modify_SPI_Reg_bits(self, 0x0020, 15, 15, 0);
    Modify_SPI_Reg_bits(self, 0x0020, 13, 13, 1);
    Modify_SPI_Reg_bits(self, 0x0020, 15, 15, 1);

    return status;
}

void Modify_SPI_Reg_bits(LMS7002M_t *self, const uint16_t address, const uint8_t msb, const uint8_t lsb, const uint16_t value)
{
    uint16_t spiDataReg = LMS7002M_spi_read(self, address); //read current SPI reg data
    uint16_t spiMask = (~(~0u << (msb - lsb + 1))) << (lsb); // creates bit mask
    spiDataReg = (spiDataReg & (~spiMask)) | ((value << lsb) & spiMask);//clear bits
    LMS7002M_regs_set(self->regs, address, spiDataReg);
    LMS7002M_spi_write(self, address, spiDataReg); //write modified data back to SPI reg
}

int CalibrateTxGainSetup(LMS7002M_t *self, const LMS7002M_chan_t channel)
{
    int status;

    uint16_t value = LMS7002M_spi_read(self, 0x0020);
    if( (value & 3) == 1)
        value = value | 0x0014;
    else
        value = value | 0x0028;
    LMS7002M_spi_write(self, 0x0020, value);

    //RxTSP
    set_addrs_to_defaultFilter(self, channel, 0x0400, 0x040F);

    set_addrs_to_defaultFilter(self, channel, 0x0440, 0x0461);

     //--- RxTSP ---
    LMS7002M_regs_spi_read(self, 0x040a); //read back to update cache
    LMS7002M_regs_spi_read(self, 0x0403);
    LMS7002M_regs_spi_read(self, 0x040c); 
    self->regs->reg_0x040a_agc_mode = 1;
    self->regs->reg_0x040a_agc_avg = 1;
    self->regs->reg_0x0403_hbd_ovr = 1;
    self->regs->reg_0x040c_cmix_byp = 1;
    
    LMS7002M_regs_spi_write(self, 0x040a);
    LMS7002M_regs_spi_write(self, 0x0403);
    LMS7002M_regs_spi_write(self, 0x040c);

    //TBB
    LMS7002M_regs_spi_read(self, 0x0108); //read back to update cache
    LMS7002M_regs_spi_read(self, 0x0105);
   
    self->regs->reg_0x0108_cg_iamp_tbb = 1;
    self->regs->reg_0x0105_loopb_tbb = 3;
    LMS7002M_regs_spi_write(self, 0x0108);
    LMS7002M_regs_spi_write(self, 0x0105);

    //RFE
    LMS7002M_regs_spi_read(self, 0x010c); //read back to update cache
   
    self->regs->reg_0x010c_en_g_rfe = 0;
    LMS7002M_regs_spi_write(self, 0x010c);
    Modify_SPI_Reg_bits(self, 0x010D, 4, 1, 0xF);  

    //RBB
    set_addrs_to_defaultFilter(self, channel, 0x0115, 0x011A);

    LMS7002M_regs_spi_read(self, 0x0115); //read back to update cache
    LMS7002M_regs_spi_read(self, 0x0118);
    LMS7002M_regs_spi_read(self, 0x0119);
    LMS7002M_regs_spi_read(self, 0x011a);
    
    self->regs->reg_0x0115_pd_lpfl_rbb = 1;
    self->regs->reg_0x0118_input_ctl_pga_rbb = 3;
    self->regs->reg_0x0119_g_pga_rbb = 12;
    self->regs->reg_0x011a_rcc_ctl_pga_rbb = 23;
    LMS7002M_regs_spi_write(self, 0x0115);
    LMS7002M_regs_spi_write(self, 0x0118);
    LMS7002M_regs_spi_write(self, 0x0119);
    LMS7002M_regs_spi_write(self, 0x011a);

    //TRF
    LMS7002M_regs_spi_read(self, 0x0100); //read back to update cache
    self->regs->reg_0x0100_en_g_trf = 0;
    LMS7002M_regs_spi_write(self, 0x0100);

    //AFE
    int val = LMS7002M_spi_read(self, 0x0082);
    const int isel_dac_afe = (val >> 13) & 0x7; 
    set_addrs_to_defaultFilter(self, channel, 0x0082, 0x0082);
    val = LMS7002M_spi_read(self, 0x0082); //read back to update cache
    val &= ~0xE000;  // clear bits 13-15
    val |= (isel_dac_afe & 0x7) << 13; // set bits 13-15 to isel_dac_afe
    LMS7002M_spi_write(self, 0x0082, val);
    LMS7002M_regs_spi_read(self, 0x0082); //read back to update cache
    
    if(channel == LMS_CHB)
    {
        self->regs->reg_0x0082_pd_rx_afe2 = 0;
        self->regs->reg_0x0082_pd_tx_afe2 = 0;
        LMS7002M_regs_spi_write(self, 0x0082);
    }

    //BIAS
    val = LMS7002M_spi_read(self, 0x0084); 
    const int rp_calib_bias = (val >> 6) & 0x1F; 
    set_addrs_to_defaultFilter(self, channel, 0x0084, 0x0084);
    self->regs->reg_0x0084_rp_calib_bias = rp_calib_bias;
    val = LMS7002M_spi_read(self, 0x0084); 
    val &= ~0x07C0;  // clear bits 6-10
    val |= (rp_calib_bias & 0x1F) << 6; // set bits 6-10 to rp_calib_bias
    LMS7002M_spi_write(self, 0x0084, val);
    LMS7002M_regs_spi_read(self, 0x0084); //read back to update cache
    //LDO
    //do nothing

    //XBUF
    //use configured xbuf settings

    //CGEN
    set_addrs_to_defaultFilter(self, channel, 0x0086, 0x008C);  

    status = LMS7002M_set_data_clock(self, 30.72e6, 61.44e6, NULL); 
    if(status != 0){
        return status;
    }

    //SXR
    LMS7002M_regs_spi_read(self, 0x011c);
    LMS7002M_regs_spi_read(self, 0x0020);
    self->regs->reg_0x0020_mac = 1;
    LMS7002M_regs_spi_write(self, 0x0020);
    self->regs->reg_0x011c_pd_vco = 1;
    LMS7002M_regs_spi_write(self, 0x011c);

    LMS7002M_set_mac_ch(self, channel);

    //TxTSP
    val = LMS7002M_spi_read(self, 0x0208);
    const int isinc = (val >> 7) & 0x0001; 
    const int txcmixGainLSB = (val >> 14) & 0x0003;
    const int txcmixGainMSB = (val >> 12) & 0x0001;
   
    set_addrs_to_defaultFilter(self, channel, 0x0200, 0x020C);
    set_addrs_to_defaultFilter(self, channel, 0x0240, 0x0261);
  
    val = LMS7002M_spi_read(self, 0x0208);
    val &= ~0xD080; // clear bits 14-15, 12, 7
    val |= (txcmixGainLSB & 0x3) << 14; // set bits 14-15 to txcmixGainLSB
    val |= (txcmixGainMSB & 0x1) << 12; // set bit 12 to txcmixGainMSB
    val |= (isinc & 0x1) << 7; // set bit 7 to isinc
    LMS7002M_spi_write(self, 0x0208, val);
    LMS7002M_regs_spi_read(self, 0x0208); //read back to update cache

    Modify_SPI_Reg_bits(self, 0x0200, 3, 3, 1);

    Modify_SPI_Reg_bits(self, 0x0200, 2, 2, 1);

    int16_t tsgValue = 0x7FFF;
    if(txcmixGainMSB == 0 && txcmixGainLSB == 1)
        tsgValue = 0x3FFF;
    else if(txcmixGainMSB == 1 && txcmixGainLSB == 0)
        tsgValue = 0x5A85;
    else
        tsgValue = 0x7FFF;
   
    self->regs->reg_0x020c_dc_reg = tsgValue;
    LMS7002M_regs_spi_write(self, 0x020c);

    LMS7002M_regs_spi_read(self, 0x0200); //read back to update cache
    self->regs->reg_0x0200_tsgdcldi = 0;
    LMS7002M_regs_spi_write(self, 0x0200);
    self->regs->reg_0x0200_tsgdcldi = 1;
    LMS7002M_regs_spi_write(self, 0x0200);
    self->regs->reg_0x0200_tsgdcldi = 0;
    LMS7002M_regs_spi_write(self, 0x0200);

    self->regs->reg_0x020c_dc_reg = tsgValue;
    LMS7002M_regs_spi_write(self, 0x020c);

    self->regs->reg_0x0200_tsgdcldq = 0;
    LMS7002M_regs_spi_write(self, 0x0200);
    self->regs->reg_0x0200_tsgdcldq = 1;
    LMS7002M_regs_spi_write(self, 0x0200);
    self->regs->reg_0x0200_tsgdcldq = 0;
    LMS7002M_regs_spi_write(self, 0x0200);

    float_type TSPClk = GetReferenceClk_TSP(self, true);
    LMS7002M_set_nco_freq(self, LMS_TX, channel,  0.5e6/TSPClk); 

    return 0;
}


float_type GetFrequencyCGEN(LMS7002M_t *self)
{
    float_type dMul = (self->cgen_fref/2.0)/(Get_SPI_Reg_bits(self, 0x0089, 10, 3)+1); //DIV_OUTCH_CGEN
    uint16_t gINT = Get_SPI_Reg_bits(self, 0x0088, 13, 0); //read whole register to reduce SPI transfers
    uint32_t gFRAC = ((gINT & 0xF) * 65536) | Get_SPI_Reg_bits(self, 0x0087, 15, 0);
    return dMul * (((gINT>>4) + 1 + gFRAC/1048576.0));
}

float_type GetReferenceClk_TSP(LMS7002M_t *self, bool tx)
{
    float_type cgenFreq = GetFrequencyCGEN(self);
	float_type clklfreq = cgenFreq/pow(2.0, Get_SPI_Reg_bits(self, 0x0089, 12, 11));
    if(Get_SPI_Reg_bits(self, 0x0086, 11, 11) == 0)
        return tx ? clklfreq : cgenFreq/4.0;
    else
        return tx ? cgenFreq : clklfreq/4.0;
}

uint16_t Get_SPI_Reg_bits(LMS7002M_t *self, uint16_t address, uint8_t msb, uint8_t lsb)
{
    return (LMS7002M_spi_read(self, address) & (~(~0u<<(msb+1)))) >> lsb; //shift bits to LSB
}

void set_addrs_to_defaultFilter(LMS7002M_t *self, const LMS7002M_chan_t channel, const int start_addr, const int stop_addr)
{
    LMS7002M_set_mac_ch(self, channel);
    for (int addr = start_addr; addr <= stop_addr; addr++)
    {
        int value = LMS7002M_regs_default_tofilter(addr);
        if (value == -1) continue; //not in map
        LMS7002M_regs_set(LMS7002M_regs(self), addr, value);
        LMS7002M_regs_spi_write(self, addr);
    }
}


static inline int LMS7002M_regs_default_tofilter(const int addr)
{
    // if (addr == 0x0020) return 0xffff;
    // if (addr == 0x0021) return 0xe9f;
    // if (addr == 0x0022) return 0x7df;
    // if (addr == 0x0023) return 0x5559;
    // if (addr == 0x0024) return 0xe4e4;
    // if (addr == 0x0025) return 0x101;
    // if (addr == 0x0026) return 0x101;
    // if (addr == 0x0027) return 0xe4e4;
    // if (addr == 0x0028) return 0x101;
    // if (addr == 0x0029) return 0x101;
    // if (addr == 0x002A) return 0x86;
    // if (addr == 0x002B) return 0x10;
    // if (addr == 0x002C) return 0xffff;
    // if (addr == 0x002E) return 0x0;
    // if (addr == 0x002F) return 0x3840;
    // if (addr == 0x0081) return 0x0;
    // if (addr == 0x0082) return 0x800b;
    // if (addr == 0x0084) return 0x400;
    // if (addr == 0x0085) return 0x1;

    if (addr == 0x0086) return 0x4901;  // yes
    if (addr == 0x0087) return 0x400;
    if (addr == 0x0088) return 0x780;
    if (addr == 0x0089) return 0x20;
    if (addr == 0x008A) return 0x514;
    if (addr == 0x008B) return 0x2100;
    if (addr == 0x008C) return 0x67b;

    // if (addr == 0x008D) return 0x0;
    // if (addr == 0x0092) return 0x1;
    // if (addr == 0x0093) return 0x0;
    // if (addr == 0x0094) return 0x0;
    // if (addr == 0x0095) return 0x0;
    // if (addr == 0x0096) return 0x0;
    // if (addr == 0x0097) return 0x0;
    // if (addr == 0x0098) return 0x0;
    // if (addr == 0x0099) return 0x6565;
    // if (addr == 0x009A) return 0x658c;
    // if (addr == 0x009B) return 0x6565;
    // if (addr == 0x009C) return 0x658c;
    // if (addr == 0x009D) return 0x6565;
    // if (addr == 0x009E) return 0x658c;
    // if (addr == 0x009F) return 0x658c;
    // if (addr == 0x00A0) return 0x6565;
    // if (addr == 0x00A1) return 0x6565;
    // if (addr == 0x00A2) return 0x6565;
    // if (addr == 0x00A3) return 0x6565;
    // if (addr == 0x00A4) return 0x6565;
    // if (addr == 0x00A5) return 0x6565;
    // if (addr == 0x00A6) return 0xf;
    // if (addr == 0x00A7) return 0x6565;
    // if (addr == 0x00a8) return 0x0;
    // if (addr == 0x00aa) return 0x0;
    // if (addr == 0x00ab) return 0x0;
    // if (addr == 0x00ad) return 0x3ff;
    // if (addr == 0x00ae) return 0xf000;
    // if (addr == 0x0100) return 0x3409;
    // if (addr == 0x0101) return 0x7800;
    // if (addr == 0x0102) return 0x3180;
    // if (addr == 0x0103) return 0xa12;
    // if (addr == 0x0104) return 0x88;
    // if (addr == 0x0105) return 0x7;
    // if (addr == 0x0106) return 0x318c;
    // if (addr == 0x0107) return 0x318c;
    // if (addr == 0x0108) return 0x9426;
    // if (addr == 0x0109) return 0x61c1;
    // if (addr == 0x010A) return 0x104c;
    // if (addr == 0x010b) return 0x0;
    // if (addr == 0x010C) return 0x88fd;
    // if (addr == 0x010D) return 0x9e;
    // if (addr == 0x010E) return 0x2040;
    // if (addr == 0x010F) return 0x3042;
    // if (addr == 0x0110) return 0xbf4;
    // if (addr == 0x0111) return 0x83;
    // if (addr == 0x0112) return 0xc0e6;
    // if (addr == 0x0113) return 0x3c3;
    // if (addr == 0x0114) return 0x8d;

    if (addr == 0x0115) return 0x9;  // yes
    if (addr == 0x0116) return 0x8180;
    if (addr == 0x0117) return 0x280c;
    if (addr == 0x0118) return 0x18c;
    if (addr == 0x0119) return 0x528b;
    if (addr == 0x011A) return 0x2e02;

    // if (addr == 0x011B) return 0x0;
    // if (addr == 0x011C) return 0xad43;
    // if (addr == 0x011D) return 0x400;
    // if (addr == 0x011E) return 0x780;
    // if (addr == 0x011F) return 0x3640;
    // if (addr == 0x0120) return 0xb9ff;
    // if (addr == 0x0121) return 0x3404;
    // if (addr == 0x0122) return 0x33f;
    // if (addr == 0x0123) return 0x67b;
    // if (addr == 0x0124) return 0x0;
    // if (addr == 0x0125) return 0x9400;
    // if (addr == 0x0126) return 0x12ff;

    if (addr == 0x0200) return 0x81;  //yes
    if (addr == 0x0201) return 0x7ff;
    if (addr == 0x0202) return 0x7ff;
    if (addr == 0x0203) return 0x0;
    if (addr == 0x0204) return 0x0;
    if (addr == 0x0205) return 0x0;
    if (addr == 0x0206) return 0x0;
    if (addr == 0x0207) return 0x0;
    if (addr == 0x0208) return 0x0070;
    if (addr == 0x0209) return 0x0;
    if (addr == 0x020a) return 0x0;
    if (addr == 0x020b) return 0x0;
    if (addr == 0x020C) return 0x0;
    if (addr == 0x0240) return 0x20;
    if (addr == 0x0241) return 0x0;
    if (addr == 0x0242) return 0x0;
    if (addr == 0x0243) return 0x0;
    if (addr == 0x0244) return 0x0;
    if (addr == 0x0245) return 0x0;
    if (addr == 0x0246) return 0x0;
    if (addr == 0x0247) return 0x0;
    if (addr == 0x0248) return 0x0;
    if (addr == 0x0249) return 0x0;
    if (addr == 0x024a) return 0x0;
    if (addr == 0x024b) return 0x0;
    if (addr == 0x024c) return 0x0;
    if (addr == 0x024d) return 0x0;
    if (addr == 0x024e) return 0x0;
    if (addr == 0x024f) return 0x0;
    if (addr == 0x0250) return 0x0;
    if (addr == 0x0251) return 0x0;
    if (addr == 0x0252) return 0x0;
    if (addr == 0x0253) return 0x0;
    if (addr == 0x0254) return 0x0;
    if (addr == 0x0255) return 0x0;
    if (addr == 0x0256) return 0x0;
    if (addr == 0x0257) return 0x0;
    if (addr == 0x0258) return 0x0;
    if (addr == 0x0259) return 0x0;
    if (addr == 0x025a) return 0x0;
    if (addr == 0x025b) return 0x0;
    if (addr == 0x025c) return 0x0;
    if (addr == 0x025d) return 0x0;
    if (addr == 0x025e) return 0x0;
    if (addr == 0x025f) return 0x0;
    if (addr == 0x0260) return 0x0;
    if (addr == 0x0261) return 0x0;
    if (addr == 0x0400) return 0x81; 
    if (addr == 0x0401) return 0x7ff;
    if (addr == 0x0402) return 0x7ff;
    if (addr == 0x0403) return 0x0;
    if (addr == 0x0404) return 0x0;
    if (addr == 0x0405) return 0x0;
    if (addr == 0x0406) return 0x0;
    if (addr == 0x0407) return 0x0;
    if (addr == 0x0408) return 0x0;
    if (addr == 0x0409) return 0x0;
    if (addr == 0x040A) return 0x1000;
    if (addr == 0x040B) return 0x0;
    if (addr == 0x040C) return 0x0038;
    if (addr == 0x040d) return 0x0;
    if (addr == 0x040e) return 0x0;
    if (addr == 0x040f) return 0x0;
    if (addr == 0x0440) return 0x20;
    if (addr == 0x0441) return 0x0;
    if (addr == 0x0442) return 0x0;
    if (addr == 0x0443) return 0x0;
    if (addr == 0x0444) return 0x0;
    if (addr == 0x0445) return 0x0;
    if (addr == 0x0446) return 0x0;
    if (addr == 0x0447) return 0x0;
    if (addr == 0x0448) return 0x0;
    if (addr == 0x0449) return 0x0;
    if (addr == 0x044a) return 0x0;
    if (addr == 0x044b) return 0x0;
    if (addr == 0x044c) return 0x0;
    if (addr == 0x044d) return 0x0;
    if (addr == 0x044e) return 0x0;
    if (addr == 0x044f) return 0x0;
    if (addr == 0x0450) return 0x0;
    if (addr == 0x0451) return 0x0;
    if (addr == 0x0452) return 0x0;
    if (addr == 0x0453) return 0x0;
    if (addr == 0x0454) return 0x0;
    if (addr == 0x0455) return 0x0;
    if (addr == 0x0456) return 0x0;
    if (addr == 0x0457) return 0x0;
    if (addr == 0x0458) return 0x0;
    if (addr == 0x0459) return 0x0;
    if (addr == 0x045a) return 0x0;
    if (addr == 0x045b) return 0x0;
    if (addr == 0x045c) return 0x0;
    if (addr == 0x045d) return 0x0;
    if (addr == 0x045e) return 0x0;
    if (addr == 0x045f) return 0x0;
    if (addr == 0x0460) return 0x0;
    if (addr == 0x0461) return 0x0;

    // if (addr == 0x05c0) return 0x0;
    // if (addr == 0x05c1) return 0x0;
    // if (addr == 0x05c2) return 0x0;
    // if (addr == 0x05c3) return 0x0;
    // if (addr == 0x05c4) return 0x0;
    // if (addr == 0x05c5) return 0x0;
    // if (addr == 0x05c6) return 0x0;
    // if (addr == 0x05c7) return 0x0;
    // if (addr == 0x05c8) return 0x0;
    // if (addr == 0x05c9) return 0x0;
    // if (addr == 0x05ca) return 0x0;
    // if (addr == 0x05cb) return 0x0;
    // if (addr == 0x05cc) return 0x0;
    // if (addr == 0x0600) return 0xf00;
    // if (addr == 0x0601) return 0x0;
    // if (addr == 0x0602) return 0x2000;
    // if (addr == 0x0603) return 0x0;
    // if (addr == 0x0604) return 0x0;
    // if (addr == 0x0605) return 0x0;
    // if (addr == 0x0606) return 0x0;
    // if (addr == 0x0640) return 0xa0;
    // if (addr == 0x0641) return 0x1020;
    return -1;
}

