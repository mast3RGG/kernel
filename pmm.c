#include "pmm.h"
#include "types.h"

static uint8_t *bitmap;
static uint64_t max_blocks;



static void bitmap_set(uint64_t bit) {

  uint64_t byte_index = bit >> 3;
  uint64_t byte_offset = bit & 7;

  bitmap[byte_index] |= 1 << byte_offset;
  
}



