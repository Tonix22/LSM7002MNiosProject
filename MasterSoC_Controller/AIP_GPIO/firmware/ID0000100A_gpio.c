#include "ID0000100A_gpio.h"
//#include "aip.h"
//#include "firmware.h"
#include <stdbool.h>
#include <stdint.h>
#include "stdlib.h"

#define ID0000100A_STATUS_BITS 8
#define ID0000100A_STATUS_BIT_DONE 0
#define ID0000100A_CONFIG_AMOUNT 5

#define MMEMIN 0    // output data register
#define AMEMIN 1
#define MMEMOUT 2   // input data register
#define AMEMOUT 3
#define MMEMOUT2 4  // register IRQ
#define AMEMOUT2 5
#define CCONFREG 6  // MODER register
#define ACONFREG 7
#define STATUS 30
#define IPID 31

// static aip_config_t ID0000100A_csv [] = {
//     {"MMEMIN", 0, 'W', 1},
//     {"MMEMOUT", 2, 'R', 1},
//     {"MMEMOUT2", 4, 'R', 1},
//     {"CCONFREG", 6, 'W', 1},
//     {"STATUS", 30, 'B', 1},
//     {"IPID", 31, 'R', 1}
// };


static int32_t ID0000100A_clearStatus(uint32_t port);


int32_t ID0000100A_init(uint32_t port)
{
    uint32_t id;

   // aip_init(port, ID0000100A_csv, ID0000100A_CONFIG_AMOUNT);

    ID0000100A_clearStatus(port);

    return 0;
}

int32_t ID0000100A_GPIO_MODER_Write(uint32_t port, uint32_t data)
{
  
    aip_writeConfReg(port, CCONFREG, &data, 1, 0);

    aip_enableINT(port, ID0000100A_STATUS_BIT_DONE);

    return 0;
}

int32_t ID0000100A_GPIO_INT_MODER_Write(uint32_t port, uint32_t data)
{
  
    aip_writeConfReg(port, CCONFREG, &data, 1, 1);

    aip_enableINT(port, ID0000100A_STATUS_BIT_DONE);

    return 0;
}
/*int32_t ID0000100A_disableGPIO(uint8_t port)
{
    uint32_t data = 0;

    aip_writeConfReg(port, CCONFREG, &data, 1, 0);

    aip_disableINT(port, 0);
        
    aip_clearINT(port, 0);

    return 0;
}*/

int32_t ID0000100A_startIP(uint32_t port)
{
    aip_start(port);

    return 0;
}

int32_t ID0000100A_wrGPIO_OUTDATA(uint32_t port, uint32_t *data, uint32_t size, uint32_t offset)
{
    aip_writeMem(port, MMEMIN, data, size, offset);

    return 0;
}

int32_t ID0000100A_readGPIO_INData(uint32_t port, uint32_t *data, uint32_t size, uint32_t offset)
{
    aip_readMem(port, MMEMOUT, data, size, offset);

    return 0;
}

int32_t ID0000100A_readEXT_INTERRUPTS(uint32_t port, uint32_t *data, uint32_t size, uint32_t offset)
{
    aip_readMem(port, MMEMOUT2, data, size, offset);

    return 0;
}

int32_t ID0000100A_getStatus(uint32_t port, uint32_t *status)
{
    aip_getStatus(port, status);

    return 0;
}

int32_t ID0000100A_waitDone(uint32_t port)
{
    uint8_t statusINT = 0;

    do
    {
        aip_getINT(port, &statusINT);
    } while (!(statusINT && 0x1));

    return 0;
}

static int32_t ID0000100A_clearStatus(uint32_t port)
{
    for(uint8_t i = 0; i < ID0000100A_STATUS_BITS; i++)
    {
        aip_disableINT(port, i);
        
        aip_clearINT(port, i);
    }

    return 0;
}
