#ifndef FUNCTIONS_AIP_H
#define FUNCTIONS_AIP_H

#include <stdint.h>

void *aip_init(void);
void aip_finish(void *aip);
void aip_write(void *aip, uint32_t *parameters, uint32_t nflits, uint32_t offset);
void aip_start(void *aip);
uint32_t aip_read(void *aip, uint32_t nflits, uint32_t offset);
void aip_reset(void *aip);
uint32_t getStatus(void *aip);
void clearDone(void *aip);
//void enableDone(void *aip);

#endif