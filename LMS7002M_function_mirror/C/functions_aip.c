#include "caip.h"

#include <stdint.h>
#include <stdio.h>

#include <conio.h>


void *aip_init(void)
{

    const char *connector = "COM7";
    uint8_t nic_addr = 1;
    uint8_t port = 0;
    const char *csv_file = "C:/WorkspaceDDigital/LSM7002MNiosProject/IpdiDriver/ID0000200F_config.csv";

    caip_t *aip = caip_init(connector, nic_addr, port, csv_file);

    aip->reset();

    return (void *)aip;
}

void aip_finish(void *aip)
{
    if (aip != NULL)
    {
        ((caip_t *)aip)->finish();
    }
}

void aip_write(void *aip, uint32_t *parameters, uint32_t nflits, uint32_t offset)
{
    if (aip != NULL)
    {
        ((caip_t *)aip)->writeMem("MProgramMEMIN", parameters, nflits, offset);
    }
}

void aip_start(void *aip)
{
    if (aip != NULL)
    {
        ((caip_t *)aip)->start();
    }
}

uint32_t aip_read(void *aip, uint32_t nflits, uint32_t offset)
{
    uint32_t ctrl_answer[1] = {0};
    if (aip != NULL)
    {
     ((caip_t *)aip)->readMem("MMEMOUT", ctrl_answer, nflits, offset);
    }
    return ctrl_answer[0];
}

void aip_reset(void *aip)
{
    if (aip != NULL)
    {
        ((caip_t *)aip)->reset();
    }
}


void clearDone(void *aip)
{
    if (aip != NULL)
    {
        ((caip_t *)aip)->clearINT(0);
    }
}

uint32_t getStatus(void *aip)
{
    uint32_t STATUS[1];
    if (aip != NULL)
    {
        ((caip_t *)aip)->getStatus(STATUS);
    }
    return STATUS[0];
}


// void enableDone(void *aip)
// {
//     if (aip != NULL)
//     {
//         ((caip_t *)aip)->enableINT(0);
//     }
// }
 

