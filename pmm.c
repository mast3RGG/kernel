#include "pmm.h"
#include "types.h"

static uint8_t *bitmap;
static uint64_t max_blocks;



static void bitmap_set(uint64_t bit) {

  uint64_t byte_index = bit >> 3;
  uint64_t byte_offset = bit & 7;

  bitmap[byte_index] |= (1 << byte_offset);
}


static void bitmap_reset(uint64_t bit) {
  uint64_t byte_index = bit >> 3;
  uint64_t byte_offset = bit & 7;
  bitmap[byte_index] &= ~(1 << byte_offset);
}

static uint8_t bitmap_test(uint64_t bit) {
  uint64_t byte_index = bit >> 3;
  uint64_t byte_offset = bit & 7;
  
  return (bitmap[byte_index] & (1 << byte_offset)) != 0;
}

void pmm_init(uint64_t mboot_addr, uint64_t mem_size) {
  max_blocks = mem_size / 4096;
  uint64_t bitmap_size = max_blocks / 8;

  bitmap = (uint8_t *)0x200000;

  for (uint64_t i = 0; i < bitmap_size; i++) {
    bitmap[i] = 0xFF;    
  }
  
  
}



