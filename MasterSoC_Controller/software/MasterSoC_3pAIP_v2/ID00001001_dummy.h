#ifndef ID00001001_DUMMY_H_
#define ID00001001_DUMMY_H_

#include <stdint.h>

int32_t ID00001001_init(uint32_t port);

int32_t ID00001001_enableDelay(uint32_t port, uint32_t msec);

int32_t ID00001001_disableDelay(uint32_t port);

int32_t ID00001001_startIP(uint32_t port);

int32_t ID00001001_writeData(uint32_t port, uint32_t *data, uint32_t size, uint32_t offset);

int32_t ID00001001_readData(uint32_t port, uint32_t *data, uint32_t size, uint32_t offset);

int32_t ID00001001_getStatus(uint32_t port, uint32_t *status);

int32_t ID00001001_waitDone(uint32_t port);

int32_t ID00001001_waitirq(uint32_t port);

#endif //ID00001001_DUMMY_H_
