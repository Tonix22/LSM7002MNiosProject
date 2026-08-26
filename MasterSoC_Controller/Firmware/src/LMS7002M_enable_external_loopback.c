
#include "LMS7002M_enable_external_loopback.h"
#include "platform.h"

void LMS7002M_enable_external_loopback (LMS7002M_t *lms, const LMS7002M_chan_t channel, const bool enable) {
   
    unsigned char txbuf[4]; 
    txbuf[0] = 0x00;   // address to read
    txbuf[1] = 0x17;

    spi_read_API(txbuf, 2, 1); 
   // printf("read txbuf[0] = %02X, txbuf[1] = %02X, txbuf[2] = %02X, txbuf[3] = %02X\n", txbuf[0], txbuf[1], txbuf[2], txbuf[3]);      

    if (channel == LMS_CHA) {
        txbuf[3] = txbuf[3] & 0xF8;
        if (enable)      
            txbuf[3] = txbuf[3] | 0x03;   
        else
            txbuf[3] = txbuf[3] | 0x04;   
        } 
    else if (channel == LMS_CHB) {
        txbuf[3] = txbuf[3] & 0x8F; 
        if (enable)  
            txbuf[3] = txbuf[3] | 0x30;   
        else
            txbuf[3] = txbuf[3] | 0x40;   
        }
    else if (channel == LMS_CHAB) {
        txbuf[3] = txbuf[3] & 0x88;
        if (enable)    
            txbuf[3] = txbuf[3] | 0x33;   
        else
            txbuf[3] = txbuf[3] | 0x44;   
        }  

       txbuf[0] = 0x80; // address to write
     // printf("txbuf[0] = %02X, txbuf[1] = %02X, txbuf[2] = %02X, txbuf[3] = %02X\n", txbuf[0], txbuf[1], txbuf[2], txbuf[3]);  
      spi_write_API(txbuf, 4, 1);    
}
