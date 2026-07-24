#ifndef PAGING_H
#define PAGING_H

#define PML4_INDEX(addr) (((addr) >> 39) & 0x1FF)
#define PDPT_INDEX(addr) (((addr) >> 30) & 0x1FF)
#define PD_INDEX(addr)   (((addr) >> 21) & 0x1FF)
#define PT_INDEX(addr)   (((addr) >> 12) & 0x1FF)
#define PAGE_OFFSET(addr) ((addr) & 0xFFF)

#endif
