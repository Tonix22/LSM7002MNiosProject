
#include "LMS7002M_RFIC_ENABLES.h"

void LMS7002M_CORE_LDO_ENABLE (LMS7002M_t *lms, const bool enable) {
   
    unsigned char txbuf[4]; 
    txbuf[0] = 0x00;   // address to read
    txbuf[1] = 0x13;

    spi_read_API(txbuf, 2, 1); 
      
    if (enable) {
        txbuf[3] = txbuf[3] | 0x04;  // Set bit 2 to enable CORE LDO
    } else {
        txbuf[3] = txbuf[3] & ~0x04; // Clear bit 2 to disable CORE LDO
    } 
    txbuf[0] = 0x80;   // address to write
    spi_write_API(txbuf, 4, 1);    
}

void LMS7002M_RXEN (LMS7002M_t *lms, const bool enable) {
    unsigned char txbuf[4]; 
    txbuf[0] = 0x00;   // address to read
    txbuf[1] = 0x13;

    spi_read_API(txbuf, 2, 1); 
      
    if (enable) {
        txbuf[3] = txbuf[3] | 0x40;  // Set bit 6 to enable RX
    } else {
        txbuf[3] = txbuf[3] & ~0x40; // Clear bit 6 to disable RX
    }
    txbuf[0] = 0x80;   // address to write
    spi_write_API(txbuf, 4, 1);    
}

void LMS7002M_TXEN (LMS7002M_t *lms, const bool enable) {
    unsigned char txbuf[4]; 
    txbuf[0] = 0x00;   // address to read
    txbuf[1] = 0x13;

    spi_read_API(txbuf, 2, 1); 
      
    if (enable) {
        txbuf[3] = txbuf[3] | 0x20;  // Set bit 5 to enable TX
    } else {
        txbuf[3] = txbuf[3] & ~0x20; // Clear bit 5 to disable TX
    }
    txbuf[0] = 0x80;   // address to write
    spi_write_API(txbuf, 4, 1);    
}

void LMS7002M_TXNRX1 (LMS7002M_t *lms, const bool enable) {
    unsigned char txbuf[4]; 
    txbuf[0] = 0x00;   // address to read
    txbuf[1] = 0x13;

    spi_read_API(txbuf, 2, 1); 
      
    if (enable) {
        txbuf[3] = txbuf[3] | 0x08;  // Set bit 3 to enable TXNRX1
    } else {
        txbuf[3] = txbuf[3] & ~0x08; // Clear bit 3 to disable TXNRX1
    }
    txbuf[0] = 0x80;   // address to write
    spi_write_API(txbuf, 4, 1);    
}

void LMS7002M_TXNRX2 (LMS7002M_t *lms, const bool enable) {
    unsigned char txbuf[4]; 
    txbuf[0] = 0x00;   // address to read
    txbuf[1] = 0x13;

    spi_read_API(txbuf, 2, 1); 
      
    if (enable) {
        txbuf[3] = txbuf[3] | 0x10;  // Set bit 4 to enable TXNRX2
    } else {
        txbuf[3] = txbuf[3] & ~0x10; // Clear bit 4 to disable TXNRX2
    }
    txbuf[0] = 0x80;   // address to write
    spi_write_API(txbuf, 4, 1);    
}

void LMS7002M_RESET (LMS7002M_t *lms) {
    unsigned char txbuf[4]; 
    txbuf[0] = 0x00;   // address to read
    txbuf[1] = 0x13;

    spi_read_API(txbuf, 2, 1); 
    
    txbuf[0] = 0x80;   // address to write

    txbuf[3] = txbuf[3] & ~0x02;  // clear bit 1 to reset the device
    spi_write_API(txbuf, 4, 1); 
    
    txbuf[3] = txbuf[3] | 0x02;  // set bit 1 to release reset
    spi_write_API(txbuf, 4, 1);   
}