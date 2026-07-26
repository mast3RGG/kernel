#include "pmm.h"
#include "multiboot.h"
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

  struct multiboot_tag *tag = (struct multiboot_tag *)(mboot_addr + 8);

  while (tag->type != 0) {
    if (tag->type == 6) {
      struct multiboot_tag_mmap *tag_mmap = (struct multiboot_tag_mmap *)tag;
      uint8_t *base = (uint8_t *)tag + sizeof(struct multiboot_tag_mmap);
      uint32_t offset = 0;

      while (offset <
             (tag_mmap->header.size - sizeof(struct multiboot_tag_mmap))) {
        struct multiboot_mmap_entry *entry =
            (struct multiboot_mmap_entry *)(base + offset);

        if (entry->type == 1) {
        }

	offset += tag_mmap->entry_size;
      }
      



    } else {
      tag = (struct multiboot_tag *) ((uint64_t)tag + (tag->size + 7) & ~7 );
    }

    
  }
}


