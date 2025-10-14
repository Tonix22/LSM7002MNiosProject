#ifndef ID00004003_MASTERSOC_H_
#define ID00004003_MASTERSOC_H_

#include <stdint.h>

int32_t ID00004003_init(uint32_t port);

int32_t ID00004003_startIP(uint32_t port);

int32_t ID00004003_writeData(uint32_t port, uint32_t *data, uint32_t size, uint32_t offset);

int32_t ID00004003_readData(uint32_t port, uint32_t *data, uint32_t size, uint32_t offset);

int32_t ID00004003_getStatus(uint32_t port, uint32_t *status);

int32_t ID00004003_waitDone(uint32_t port);


#endif //ID00004003_MASTERSOC_H_
