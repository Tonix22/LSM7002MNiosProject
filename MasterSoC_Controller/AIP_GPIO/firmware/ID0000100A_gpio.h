#ifndef ID0000100A_GPIO_H_
#define ID0000100A_GPIO_H_

//#include "aip.h"

#include <stdint.h>

int32_t ID0000100A_init(uint32_t port);

int32_t ID0000100A_GPIO_MODER_Write(uint32_t port, uint32_t data);

int32_t ID0000100A_GPIO_INT_MODER_Write(uint32_t port, uint32_t data);

//int32_t ID0000100A_disableGPIO(uint8_t port);

int32_t ID0000100A_startIP(uint32_t port);

int32_t ID0000100A_wrGPIO_OUTDATA(uint32_t port, uint32_t *data, uint32_t size, uint32_t offset);

int32_t ID0000100A_readGPIO_INData(uint32_t port, uint32_t *data, uint32_t size, uint32_t offset);

int32_t ID0000100A_readEXT_INTERRUPTS(uint32_t port, uint32_t *data, uint32_t size, uint32_t offset);

int32_t ID0000100A_getStatus(uint32_t port, uint32_t *status);

int32_t ID0000100A_waitDone(uint32_t port);

#endif //ID0000100A_GPIO_H_