#ifndef PMM_H
#define PMM_H
#include "types.h"


void pmm_init(uint64_t mboot_addr, uint64_t mem_size);
uint64_t *pmm_alloc_block(void);
void pmm_free_block (void *);

#endif
