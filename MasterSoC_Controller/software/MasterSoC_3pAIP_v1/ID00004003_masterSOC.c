#include "ID00004003_masterSOC.h"
#include "aip.h"


#define ID00004003_STATUS_BITS 8
#define ID00004003_STATUS_BIT_DONE 0
#define ID00004003_CONFIG_AMOUNT 5

#define MDATAIN    0
#define MDATAOUT   2
#define CDELAY     4
#define STATUS     30
#define IPID       31


static int32_t ID00004003_clearStatus(uint32_t port);


int32_t ID00004003_init(uint32_t port)
{
    uint32_t id;

    //aip_init(port, ID00004003_csv, ID00004003_CONFIG_AMOUNT);

    aip_getID(port, &id);
    printf("The ID is: %x \n", id );
    //ID00004003_clearStatus(port);

    return 0;
}

int32_t ID00004003_startIP(uint32_t port)
{
    aip_start(port);

    return 0;
}

int32_t ID00004003_writeData(uint32_t port, uint32_t *data, uint32_t size, uint32_t offset)
{
    aip_writeMem(port, MDATAOUT, data, size, offset);

    return 0;
}

int32_t ID00004003_readData(uint32_t port, uint32_t *data, uint32_t size, uint32_t offset)
{
    aip_readMem(port, MDATAIN, data, size, offset);

    return 0;
}

int32_t ID00004003_getStatus(uint32_t port, uint32_t *status)
{
    aip_getStatus(port, status);

    return 0;
}

int32_t ID00004003_waitDone(uint32_t port)
{
    uint32_t statusINT = 0;

    do
    {
        aip_getINT(port, &statusINT);
    } while (!(statusINT && 0x1));

    return 0;
}

static int32_t ID00004003_clearStatus(uint32_t port)
{
    for(uint32_t i = 0; i < ID00004003_STATUS_BITS; i++)
    {
        aip_disableINT(port, i);
        
        aip_clearINT(port, i);
    }

    return 0;
}






